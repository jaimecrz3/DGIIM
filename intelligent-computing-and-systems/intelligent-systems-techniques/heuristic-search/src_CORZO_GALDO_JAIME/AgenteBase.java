package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.ArrayList;
import java.util.BitSet;

import core.game.Observation;
import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

/**
 * Clase abstracta que sirve como plantilla para los agentes que implementan el algoritmo de busqueda
 * en produndidad, A*, RTA* y LRTA*(k). Incluye atributos y metodos implementados que son heredados y pueden ser 
 * usados por estas cuatro clases.
 * Los atributos incluyen la informacion inicial del agente e informacion estatica del mapa, indices de
 * monedas, llaves y catapultas y la posicion del portal.
 * El constructor inicializa el estadoInicial ademas de toda la informacion del mapa.
 * Se incluye tambien el metodo aplicarAccion que imula exactamente un tick del juego sobre una copia del estado
 * ademas de todos los helpers que se usan en el mismo.
 * 
 * Con esta clase conseguimos generalizar todos los elementos comunes a los cuatro algoritmos, ademas de dar
 * una base a los posibles nuevos algoritmos que queramos crear para resolver el juego Catapults.
 */
public abstract class AgenteBase extends AbstractPlayer {

    // Datos del agente
    // Usamos protected para que se permita el acceso no solo desde esta clase pero en subclases tambien
    protected Estado estadoInicial;

    // Información estática del mapa
    protected int filas;
    protected int columnas;
    protected int blockSize; // Numero de píxeles que forman un bloque en el juego
    boolean[][] matrizMuros; // True en las posiciones de mapa donde hay muros(árboles)
    boolean[][] matrizAgua; // True en las posiciones de mapa donde hay agua
    protected int[][] dirCatapulta;   // -1 si no hay, 0 este, 1 sur, 2 norte, 3 oeste

    // Tipo de sprite(itype) de las distintas observaciones que puede haber en los mapas
    protected final int tipoMoneda = 15;
    protected final int tipoLlave = 16;
    protected final int tipoArbol = 0;
    protected final int tipoAgua = 3;
    protected final int tipoCatapultaEste = 7;
    protected final int tipoCatapultaSur = 5;
    protected final int tipoCatapultaNorte = 6;
    protected final int tipoCatapultaOeste = 8;
    //protected final int tipoPortal = 18;
    //protected final int tipoSuelo = 2;

    // Posicion del objetivo
    protected int portalX;
    protected int portalY;

    // Estructuras de datos estaticas que van a estar a -1 en las posiciones del mapa donde 
    // no haya el objeto correspondiente (moneda, llave o catapulta) y en las posiciones en las que si
    // se encuentre ese objeto tendran un indice de 0 a N-1, donde N es el numero de ocurrencias
    // de ese objeto en el mapa.
    // POR QUÉ: Para usar la estructura BitSet definida en Estado, necesitamos que cada objeto tenga un indice
    protected int[][] indiceMoneda;
    protected int[][] indiceLlave;
    protected int[][] indiceCatapulta;

    // Numero de ocurrencias del objeto correspondiente (moneda, llave o catapulta) en el mapa
    protected int numMonedas;
    protected int numLlaves;
    protected int numCatapultas;

    // Constante para almacenar el orden definido para la generacion de sucesores, para evitar aleatoridad
    protected static final ACTIONS[] ORDEN_EXPANSION = {
        ACTIONS.ACTION_RIGHT,
        ACTIONS.ACTION_UP,
        ACTIONS.ACTION_LEFT,
        ACTIONS.ACTION_DOWN
    };

    // Constructor
    public AgenteBase(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super();
        inicializarMapaYDatosEstaticos(stateObs);
        this.estadoInicial = construirEstadoInicial(stateObs);
    }

    // Aqui guardamos lo estático: tamaño del mapa, portal, muros, 
    // agua y dirección de cada catapulta
    // Se llama desde el constructor de la clase
    protected void inicializarMapaYDatosEstaticos(StateObservation stateObs) {
        this.blockSize = stateObs.getBlockSize(); // Cuantos píxeles forman un bloque en el juego
        // Obtenemos el tamaño del mapa (en bloques del juego)
        // Realmente son las columnas, pero como se indican primero en las estrcuturas de datos las llamamos filas 
        // (siguiendo el ejemplo de CamlRace de las transparencias)
        this.filas = stateObs.getObservationGrid().length; 
        this.columnas = stateObs.getObservationGrid()[0].length;

        this.matrizMuros = new boolean[filas][columnas];
        this.matrizAgua = new boolean[filas][columnas];
        this.dirCatapulta = new int[filas][columnas];

        this.indiceMoneda = new int[filas][columnas];
        this.indiceLlave = new int[filas][columnas];
        this.indiceCatapulta = new int[filas][columnas];

        for(int x = 0; x < filas; x++){
            for(int y = 0; y < columnas; y++){
                dirCatapulta[x][y] = -1;
                indiceMoneda[x][y] = -1;
                indiceLlave[x][y] = -1;
                indiceCatapulta[x][y] = -1;
            }
        }

        // Obtenemos la posición de los portales
        // 1. Obtenemos la estructura con todos los portales
        ArrayList<Observation>[] posicionesPortales = stateObs.getPortalsPositions();
        
        // 2. Buscamos la lista que no esté vacía
        if (posicionesPortales != null) {
            for (ArrayList<Observation> listaTipoPortal : posicionesPortales) {
                if (listaTipoPortal != null && !listaTipoPortal.isEmpty()) {
                    // Como sabemos que solo hay una meta, cogemos la primera
                    Observation portalObs = listaTipoPortal.get(0);
                    
                    // 3. Convertimos de píxeles a coordenadas del grid 
                    this.portalX = (int) (portalObs.position.x / this.blockSize);
                    this.portalY = (int) (portalObs.position.y / this.blockSize);
                    
                    break; // Ya tenemos el portal, salimos del bucle
                }
            }
        }

        // Inmovibles: muros, agua, catapultas  
        // getImmovablePositions() devuelve una lista de observaciones de sprites inmovibles en el juego.
        // Como puede haber de distinto tipo, cada entrada del array corresponde a un tipo de sprite  
        ArrayList<Observation>[] immovables = stateObs.getImmovablePositions();
        numCatapultas = 0;
        
        if(immovables != null){
            for(ArrayList<Observation> grupo : immovables){
                if(grupo == null || grupo.isEmpty()) continue;

                for(Observation obs : grupo){
                    int x = (int) (obs.position.x / this.blockSize);
                    int y = (int) (obs.position.y / this.blockSize);

                    if(obs.itype == tipoArbol){
                        matrizMuros[x][y] = true;
                    }
                    else if(obs.itype == tipoAgua){
                        matrizAgua[x][y] = true;
                    }
                    else if(obs.itype == tipoCatapultaEste){
                        dirCatapulta[x][y] = 0;
                        indiceCatapulta[x][y] = numCatapultas;
                        numCatapultas++;
                    }
                    else if(obs.itype == tipoCatapultaSur){
                        dirCatapulta[x][y] = 1;
                        indiceCatapulta[x][y] = numCatapultas;
                        numCatapultas++;
                    }
                    else if(obs.itype == tipoCatapultaNorte){
                        dirCatapulta[x][y] = 2;
                        indiceCatapulta[x][y] = numCatapultas;
                        numCatapultas++;
                    }
                    else if(obs.itype == tipoCatapultaOeste){
                        dirCatapulta[x][y] = 3;
                        indiceCatapulta[x][y] = numCatapultas;
                        numCatapultas++;
                    }
                }
            }
        }

        // Recursos: solo para asignar índices
        ArrayList<Observation>[] resources = stateObs.getResourcesPositions();
        numMonedas = 0;
        numLlaves = 0;

        if (resources != null) {
            for (ArrayList<Observation> grupo : resources) {
                if (grupo == null || grupo.isEmpty()) continue;

                for (Observation obs : grupo) {
                    int x = (int) (obs.position.x / this.blockSize);
                    int y = (int) (obs.position.y / this.blockSize);

                    if (obs.itype == tipoMoneda) {
                        indiceMoneda[x][y] = numMonedas;
                        numMonedas++;
                    }
                    else if (obs.itype == tipoLlave) {
                        indiceLlave[x][y] = numLlaves;
                        numLlaves++;
                    }
                }
            }
        }
    }

    // Aquí guardamos lo dinámico al comienzo de la partida: 
    // posición inicial del avatar, inventarío vacío, monedas, catapultas
    // y llaves que quedan
    // Este metodo se llama desde el constructor de la clase
    protected Estado construirEstadoInicial(StateObservation stateObs) {
        Estado e = new Estado();

        Vector2d posAvatar = stateObs.getAvatarPosition();
        e.x = (int) (posAvatar.x/this.blockSize);
        e.y = (int) (posAvatar.y/this.blockSize);
        e.tieneLlave = false;
        e.monedas = 0;
        e.enVuelo = false;
        e.dirVuelo = -1;
        e.pendienteTransformacion = false;
        
        // Inicializamos los objetos BitSet con un tamaño inicial suficiente para representar bits desde el índice 0 hasta 
        // numMonedas - 1 / numCatapultas - 1 / numLlaves - 1
        e.monedasRestantes = new BitSet(numMonedas);
        e.catapultasRestantes = new BitSet(numCatapultas);
        e.llavesRestantes = new BitSet(numLlaves);

        // Al principio existen todas las monedas
        for (int i = 0; i < numMonedas; i++) {
            // Llamamos al método set(int bitIndex) para cambiar el valor del bit en la posición i de verdadero, 
            // indicando que ese elemento específico está activo o presente.
            e.monedasRestantes.set(i);
        }

        // Al principio existen todas las llaves
        for (int i = 0; i < numLlaves; i++) {
            // Llamamos al método set(int bitIndex) para cambiar el valor del bit en la posición i de verdadero, 
            // indicando que ese elemento específico está activo o presente.
            e.llavesRestantes.set(i);
        }

        // Al principio existen todas las catapultas
        for (int i = 0; i < numCatapultas; i++) {
            // Llamamos al método set(int bitIndex) para cambiar el valor del bit en la posición i de verdadero, 
            // indicando que ese elemento específico está activo o presente.
            e.catapultasRestantes.set(i);
        }

        return e;
    }

    protected boolean esObjetivo(Estado estado) {
        // Estar en la casilla meta con llave
        return estado.x == portalX && estado.y == portalY && estado.tieneLlave;
    }

    protected boolean dentroMapa(int x, int y){
        return x >= 0 && x < filas && y >= 0 && y < columnas;
    }

    protected boolean hayMuro(int x, int y){
        return matrizMuros[x][y];
    }

    protected boolean hayAgua(int x, int y){
        return matrizAgua[x][y];
    }

    protected int direccionCatapulta(int x, int y){
        return dirCatapulta[x][y];
    }

    protected boolean hayMoneda(Estado e, int x, int y) {
        int idx = indiceMoneda[x][y];
        return idx != -1 && e.monedasRestantes.get(idx);
    }
    
    protected boolean hayLlave(Estado e, int x, int y) {
        int idx = indiceLlave[x][y];
        return idx != -1 && e.llavesRestantes.get(idx);
    }
    
    protected boolean hayCatapulta(Estado e, int x, int y) {
        int idx = indiceCatapulta[x][y];
        return idx != -1 && e.catapultasRestantes.get(idx);
    }

    protected boolean esPortal(int x, int y){
        return (x == portalX && y == portalY);
    }

    // Recoge los objetos presentes en la casilla (x,y).
    // - Monedas: solo si el inventario tiene menos de 5.
    // - Llave: se recoge siempre y desaparece del mapa.
    // En caso de recoger el objeto, se copia la matriz del objeto correspondiente
    // al nuevo estado para poder modificarla sin afectar a la matriz del estado anterior
    protected void recogerObjetos(Estado estadoActual, Estado nuevo, int x, int y) {
        int idxMoneda = indiceMoneda[x][y]; // Obtenemos el indice entero unico de la moneda en esta casilla
        // Si hay moneda, no hemos llegado al límite y el bit correspondiente está a 1 (existe)
        if (hayMoneda(estadoActual, x, y) && nuevo.monedas < 5) {
            nuevo.monedas++;
            // Hacemos una clonación del BitSet (muy rápido) y ponemos a 0 (clear)
            // el bit de esta moneda para indicar que ya no existe en el nuevo estado.
            nuevo.monedasRestantes = (BitSet) estadoActual.monedasRestantes.clone();
            nuevo.monedasRestantes.clear(idxMoneda);
        }
    
        int idxLlave = indiceLlave[x][y]; // Obtenemos el indice entero unico de la llave en esta casilla
        // Si hay llave y el bit correspondiente está a 1 (existe)
        if (hayLlave(estadoActual, x, y)) {
            nuevo.tieneLlave = true;
            // Hacemos una clonación del BitSet (muy rápido) y ponemos a 0 (clear)
            // el bit de esta llave para indicar que ya no existe en el nuevo estado.
            nuevo.llavesRestantes = (BitSet) estadoActual.llavesRestantes.clone();
            nuevo.llavesRestantes.clear(idxLlave);
        }
    }
    
    protected int siguienteXSegunDireccion(int x, int dir) {
        switch (dir) {
            case 0: return x + 1; // este
            case 3: return x - 1; // oeste
            default: return x;
        }
    }
    
    protected int siguienteYSegunDireccion(int y, int dir) {
        switch (dir) {
            case 1: return y + 1; // sur
            case 2: return y - 1; // norte
            default: return y;
        }
    }

    // Simula exactamente un tick del juego sobre una copia del estado.
    // Devuelve:
    // - un nuevo Estado si la transición es válida,
    // - null si la acción no es válida o provoca muerte.
    //
    // Casos:
    // 1) Movimiento normal del vampiro.
    // 2) Tick NIL para transformación inicial al entrar en catapulta.
    // 3) Tick NIL extra al sobrepasar otra catapulta en vuelo.
    // 4) Avance automático del murciélago con ACTION_NIL.
    protected Estado aplicarAccion(Estado estadoActual, ACTIONS accion) {

        //Estado nuevo = new Estado(estadoActual);

        // Copiamos la informacion del estado anterior al nuevo
        Estado nuevo = new Estado();
        nuevo.x = estadoActual.x;
        nuevo.y = estadoActual.y;
        nuevo.tieneLlave = estadoActual.tieneLlave;
        nuevo.monedas = estadoActual.monedas;
        nuevo.enVuelo = estadoActual.enVuelo;
        nuevo.dirVuelo = estadoActual.dirVuelo;
        nuevo.pendienteTransformacion = estadoActual.pendienteTransformacion;

        // compartir por defecto: solo la direccion, en el caso que se vaya a modificar
        // alguna de estas estracturas, ya si realizamos la copia completa, de esta manera se
        // consigue que el algoritmo sea mas eficiente
        nuevo.monedasRestantes = estadoActual.monedasRestantes;
        nuevo.catapultasRestantes = estadoActual.catapultasRestantes;
        nuevo.llavesRestantes = estadoActual.llavesRestantes;
        
        // CASO 1: Vampiro (no vuela, no pendiente de transformarse)
        if (!estadoActual.enVuelo && !estadoActual.pendienteTransformacion) {

            // En estado normal ACTION_NIL no genera transición útil.
            if (accion == ACTIONS.ACTION_NIL) {
                return null;
            }

            // Calculamos la nueva posicion segun la accion
            int nx = estadoActual.x;
            int ny = estadoActual.y;

            switch (accion) {
                case ACTION_RIGHT:
                    nx = estadoActual.x + 1;
                    break;
                case ACTION_UP:
                    ny = estadoActual.y - 1;
                    break;
                case ACTION_LEFT:
                    nx = estadoActual.x - 1;
                    break;
                case ACTION_DOWN:
                    ny = estadoActual.y + 1;
                    break;
                default:
                    return null;
            }

            if (!dentroMapa(nx, ny)) return null; // Si la nueva posicion esta fuera de los confines del mapa
            if (hayMuro(nx, ny)) return null; // Si la nueva posicion es un muro (árbol)
            if (hayAgua(nx, ny)) return null; // andando al agua = muerte
            if (esPortal(nx, ny) && !estadoActual.tieneLlave) return null; // puerta cerrada

            // En caso de ser una casilla transitable, actualizamos la posicion
            nuevo.x = nx;
            nuevo.y = ny;

            // En caso de haber monedas o llaves se intentan recoger
            recogerObjetos(estadoActual, nuevo, nx, ny);

            // Si pisa una catapulta andando:
            // - debe tener al menos 1 moneda,
            // - la catapulta desaparece,
            // - se consume una moneda,
            // - queda pendiente la transformación a murciélago.
            if (hayCatapulta(estadoActual, nx, ny)) {
                if (estadoActual.monedas == 0) {
                    return null; // usar catapulta sin monedas = muerte
                }

                nuevo.monedas--;
                // Actualizamos la matriz de las catapultas del nuevo estado
                int idxCat = indiceCatapulta[nx][ny];
                nuevo.catapultasRestantes = (BitSet) estadoActual.catapultasRestantes.clone();
                nuevo.catapultasRestantes.clear(idxCat);
                nuevo.dirVuelo = direccionCatapulta(nx, ny);
                nuevo.pendienteTransformacion = true;
            }

            return nuevo;
        }

        // CASO 2: Tick NIL de transformación inicial (vampiro -> murciélago)
        if (!estadoActual.enVuelo && estadoActual.pendienteTransformacion) {
            if (accion != ACTIONS.ACTION_NIL) return null;

            nuevo.enVuelo = true;
            nuevo.pendienteTransformacion = false;
            return nuevo;
        }

        // CASO 3: Tick NIL extra por sobrepasar catapulta en vuelo
        if (estadoActual.enVuelo && estadoActual.pendienteTransformacion) {
            if (accion != ACTIONS.ACTION_NIL) return null;

            nuevo.pendienteTransformacion = false;
            return nuevo;
        }

        // CASO 4: Murciélago en vuelo normal
        if (estadoActual.enVuelo) {
            if (accion != ACTIONS.ACTION_NIL) return null;

            // Calculamos la nueva posicion segun la direccion de vuelo
            int nx = siguienteXSegunDireccion(estadoActual.x, estadoActual.dirVuelo);
            int ny = siguienteYSegunDireccion(estadoActual.y, estadoActual.dirVuelo);

            // Si choca, no avanza y vuelve a vampiro en la casilla actual
            if (!dentroMapa(nx, ny) || hayMuro(nx, ny) || (esPortal(nx, ny) && !estadoActual.tieneLlave)) {
                nuevo.enVuelo = false;
                nuevo.dirVuelo = -1;
                nuevo.pendienteTransformacion = false;

                // Si la casilla previa a la colision era agua, al volver a vampiro muere
                if (hayAgua(estadoActual.x, estadoActual.y)){
                    return null;
                }

                return nuevo;
            }

            // En caso contrario, avanza una casilla
            nuevo.x = nx;
            nuevo.y = ny;

            // En vuelo sí puede recoger monedas y llave
            recogerObjetos(estadoActual, nuevo, nx, ny);

            // Si sobrevuela una nueva catapulta:
            // - desaparece,
            // - cambia la dirección,
            // - se consume un NIL extra en el siguiente tick.
            if (hayCatapulta(estadoActual, nx, ny)) {
                int idxCat = indiceCatapulta[nx][ny];
                nuevo.catapultasRestantes = (BitSet) estadoActual.catapultasRestantes.clone();
                nuevo.catapultasRestantes.clear(idxCat);
                nuevo.dirVuelo = direccionCatapulta(nx, ny);
                nuevo.pendienteTransformacion = true; // NIL extra por sobrepasarla
            }

            return nuevo;
        }

        return null;
    }

}