package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

/**
 * Clase que implementa el agente basado en el algoritmo de búsqueda A* (A-Estrella).
 * * QUÉ HACE:
 * Calcula de forma secuencial y determinista el plan de acciones óptimo que debe ejecutar 
 * el agente (alternando entre su forma de vampiro y murciélago) para alcanzar el portal 
 * de salida. Se trata de una búsqueda "offline": el árbol de búsqueda y el plan completo 
 * se computan en el primer tick de reloj, antes de comenzar la ejecución de las acciones.
 * * CÓMO LO HACE:
 * El algoritmo explora el mapa separando la lógica en "Estados" (que representan la 
 * configuración exacta del mundo en un momento dado: posición, inventario, vuelo) y 
 * "Nodos" (que guardan la traza del árbol de búsqueda, el coste acumulado g, la heurística h 
 * y la acción que llevó a él). 
 * Para gestionar los nodos descubiertos por explorar ("Abiertos"), emplea una estructura dual:
 * una PriorityQueue y un HashMap. Para los nodos ya expandidos ("Cerrados"), utiliza 
 * un segundo HashMap. El proceso iterativo extrae siempre el nodo con menor coste f(n), 
 * genera sus sucesores válidos al vuelo y actualiza las estructuras hasta encontrar la meta 
 * o vaciar la cola de abiertos.
 * * POR QUÉ (Decisiones de diseño y optimización para el rendimiento):
 * - Estructura dual en Abiertos (PriorityQueue + HashMap): La cola de prioridad es ideal 
 * para extraer el nodo más prometedor rápidamente, pero verificar si un estado ya existe 
 * en ella o actualizar su coste requiere un tiempo lineal O(N). Al respaldarla con un HashMap, 
 * las comprobaciones de existencia y la recuperación de nodos para actualizar rutas más 
 * cortas se realizan en tiempo constante O(1), eliminando el principal cuello de botella.
 * - HashMap en Cerrados (frente a listas): Garantiza que verificar si un estado ya fue 
 * visitado cueste O(1) en lugar del O(N) que requeriría iterar sobre un ArrayList.
 * - Uso de BitSet (frente a boolean[][] o listas de objetos): El estado del juego requiere 
 * saber qué monedas, llaves y catapultas siguen activas. Representar esto con matrices 
 * implicaba un alto coste computacional al clonar el estado para cada sucesor. `BitSet` 
 * comprime esta información a nivel de bits, permitiendo copias y mutaciones casi instantáneas 
 * con un consumo de memoria mínimo.
 * - Generación de sucesores "al vuelo": En lugar de instanciar y devolver una lista 
 * temporal de nodos sucesores en cada expansión, los sucesores se evalúan y encolan 
 * directamente. Esto reduce drásticamente la creación de "basura" en memoria, aliviando 
 * el trabajo del recolector de basura (Garbage Collector) y acelerando el tiempo total.
 */
public class AgenteAStar extends AgenteBase {

    // Plan
    private ArrayList<ACTIONS> plan;
    // índice de la siguiente acción del plan que se devuelve en act() con la funcion calcularProfundidad() 
    private int indicePlan;  
    private boolean planCalculado; // True una vez que se ha calculado el plan con la funcion calcularProfundidad() 
    private boolean metricasImpresa; // True si se han impreso las métricas con la clase metricsProvider

    // Métricas 
    private int nodosExpandidos; // Indica el numero de nodos a los que se les ha caculado los sucesores
    private int nodosAbiertosFinal; // Indica el numero de nodos en abiertos al final de la ejecucion
    private int nodosCerradosFinal; // Indica el numero de nodos en cerrados al final de la ejecucion
    private long tiempoMilisegundos; // Tiempo en calcular el plan

    // Resultado de la búsqueda 
    private Nodo nodoObjetivoEncontrado;

    // nos sirve para actualizar el valor de ordenInsercion de cada Nodo que entra en abiertos,
    // de tal manera que los que entren antes tengan un valor mas bajo y por lo tanto mas prioridad 
    // para posibles desempates.
    // Se inicializa a 0 y cada vez que entra un nodo en abiertos aumenta en 1 su valor
    private long contadorInsercion;

    // Constructor
    public AgenteAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // El constructor puede inicializar todas las estructuras iniciales y hasta el nodo inicial pero no puede hacer 
        // nada del proceso de búsqueda
        this.plan = new ArrayList<>();
        this.indicePlan = 0;
        this.planCalculado = false;
        this.metricasImpresa = false;

        this.nodosExpandidos = 0;
        this.nodosAbiertosFinal = 0;
        this.nodosCerradosFinal = 0;
        this.tiempoMilisegundos = 0;
        this.nodoObjetivoEncontrado = null;

        this.contadorInsercion = 0;
    }

    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        // Este método debe encargarse de computar el plan y devolver la siguiente acción
        // En cada tick, el motor del juego llama a act, y este no puede tardar mas de 50 ms
        // si no se pierde, y si tarda entre 40 y 50 ms, ambos incluido se ignora la acción
        // calculada y se realiza un ACTIONS.NIL

        // La búsqueda offline se hace solo en la primera llamada a act()
        if (!planCalculado) {
            long tInicio = System.nanoTime();

            calcularPlanAStar();

            long tFin = System.nanoTime();
            tiempoMilisegundos = (tFin - tInicio) / 1_000_000L;

            planCalculado = true;

            // Al encontrar el plan se deben fijar TODAS las métricas complementarias correspondientes al algoritmo
            MetricsProvider.getInstance().setAgente("AgenteAStar");
            MetricsProvider.getInstance().setNumAccionesPlan(plan.size());
            MetricsProvider.getInstance().setNodosExpandidos(nodosExpandidos);
            MetricsProvider.getInstance().setNodosAbiertos(nodosAbiertosFinal);
            MetricsProvider.getInstance().setNodosCerrados(nodosCerradosFinal);
            MetricsProvider.getInstance().setTiempoMilisegundos(tiempoMilisegundos);

            if (!metricasImpresa) {
                // Y se deben mostrar los resultados por pantalla mediante este método (sólo una vez al encontrar el plan)
                MetricsProvider.getInstance().printMetrics();
                metricasImpresa = true;
            }
        }

        if (indicePlan < plan.size()) {
            ACTIONS siguiente = plan.get(indicePlan);
            indicePlan++;
            return siguiente;
        }

        return ACTIONS.ACTION_NIL;
    }

    // La funcion heuristica es la distancia Manhattan desde el estado actual hasta el portal de salida
    private int heuristica(Estado e) {
        return Math.abs(e.x - portalX) + Math.abs(e.y - portalY);
    }

    // Definimos la regla regla de ordenación personalizada para objetos de la clase Nodo.
    // En A* los nodos se visitan según el valor de f(n), desempatando por h(n) y si también hay un empate en h(n)
    // se dará prioridad a los nodos más antiguos de la lista de abiertos. Por ultima, para los vecinos de un nodo, 
    // se usará el orden de expansión: DERECHA, ARRIBA, IZQUIERDA, ABAJO para cualquier desempate final.
    //
    // Lo que hacemos es crear un metodo que devuelve un objeto comparador. Utilizamos una funcion lamda (anonima) que
    // toma dos nodos para compararlos entre si.
    //
    // Con este comparador lo que conseguimos es eliminar aleatoridad en los resultados y que los resultados sean predecibles
    private Comparator<Nodo> comparadorAbiertos() {
        return (a, b) -> {
            // Integer.compare compara dos int y devuelve 0 si son iguales,
            // un valor menor de 0 si el primer argumento es menor que el segundo
            // y un valor mayor de 0 si el primer arguento es mayor que el segundo
            // Al usar este metodo evitamos usar el comparador == que compara referencias en vez de valores
            if (a.f != b.f) return Integer.compare(a.f, b.f);
            if (a.h != b.h) return Integer.compare(a.h, b.h);
            return Long.compare(a.ordenInsercion, b.ordenInsercion);
        };
    }

    /**
     * Bucle principal del algoritmo de búsqueda A*
     * * QUÉ HACE:
     * Partiendo del estado inicial, explora el espacio de estados hasta encontrar un nodo objetivo (llegar al portal con la llave).
     * * CÓMO LO HACE:
     * 1. Inicializa las estructuras de datos: PriorityQueue (abiertosCola) para extraer eficientemente el mínimo f(n), 
     * un HashMap (abiertos) para consultas O(1), y un HashMap (cerrados) para los nodos ya expandidos.
     * 2. Entra en un bucle mientras que queden nodos por visitar en abiertos: extrae el nodo más prometedor de 
     * abiertos y lo transfiere a cerrados.
     * 3. Verifica si es el estado objetivo. Si lo es, se termina.
     * 4. Si no es objetivo, determina qué acciones son válidas. Si el avatar está volando o 
     * en plena transformación, restringe la acción a ACTION_NIL. Si es un 
     * vampiro normal, expande hacia las 4 direcciones llamando a procesarSucesor en el orden establecido: 
     * DERECHA, ARRIBA, IZQUIERDA, ABAJO.
     * * POR QUÉ:
     * Al detectar si el estado está enVuelo o pendienteTransformacion, 
     * se evita generar y evaluar sucesores innecesarios cuando el avatar 
     * solo puede realizar un ACTION_NIL. Esto reduce el factor de ramificación 
     * del árbol, ahorrando memoria y tiempo de CPU.
     */
    private void calcularPlanAStar() {
        // Para los nodos en abiertos usamos una PriorityQueue para sacar el mejor candidato
        // segun el criterio de desempate establecido (menor f, luego menor h, luego orden de inserción)
        PriorityQueue<Nodo> abiertosCola = new PriorityQueue<>(comparadorAbiertos());
        // Para almecenar abiertos y cerrados vamos a usar un mapa, ya que 
        // necesitamos para cada estado saber si esta ya en abiertos o cerrados pero ademas
        // en A* importa el nodo asociado al estado, ya que un mismo estado puede estar representado
        // por distintos nodos, pero a los que se ha llegado por secuencias de acciones distintas
        // y con distinto coste
        HashMap<Estado, Nodo> abiertos = new HashMap<>();
        HashMap<Estado, Nodo> cerrados = new HashMap<>();

        Nodo inicial = new Nodo(estadoInicial, null, ACTIONS.ACTION_NIL, 0);
        inicial.g = 0;
        inicial.h = heuristica(estadoInicial);
        inicial.f = inicial.g + inicial.h;
        inicial.ordenInsercion = contadorInsercion;
        contadorInsercion++;

        abiertosCola.add(inicial);
        abiertos.put(inicial.estado, inicial);

        nodoObjetivoEncontrado = null;

        // Mientras queden nodos por visitar en la lista de abiertos
        while (!abiertosCola.isEmpty()) {

            // Cogemos el elemento con mas prioridad de abiertos y lo eliminamos tanto de la cola de abiertos
            // como del mapa de abiertos
            Nodo actual = abiertosCola.poll();
            abiertos.remove(actual.estado);

            // Comprobamos si hemos llegado al portal con la llave
            if (esObjetivo(actual.estado)) {
                // El nodo objetivo ya ha sido expandido asi que lo guardamos y acabamos
                nodoObjetivoEncontrado = actual;
                break;
            }

            // Si no es objetivo, lo metemos en cerrados (asociamos el valor del nodo actual con su key en el hashmap, que este caso es su estado)
            cerrados.put(actual.estado, actual);

            nodosExpandidos++; // Como se calculan los sucesores incrementamos el numero de nodos expandidos

            // Disitinguimos entre que el agente este volando, transformandose entre vampiro y murcielago
            // y cambiando de direccion en una catapulta, en cuyo caso la unica accion a expandir es ACTION_NIL,
            // y el resto de casos que se expande en el orden: DEREHA, ARRIBA, IZQUIERDA, ABAJO 
            if (actual.estado.enVuelo || actual.estado.pendienteTransformacion) {
                procesarSucesor(actual, ACTIONS.ACTION_NIL, abiertosCola, abiertos, cerrados);
                continue; // Ya no hay que procesar mas acciones, asi que pasamos al siguiente nodo del bucle while
            }
        
            procesarSucesor(actual, ACTIONS.ACTION_RIGHT, abiertosCola, abiertos, cerrados);
            procesarSucesor(actual, ACTIONS.ACTION_UP, abiertosCola, abiertos, cerrados);
            procesarSucesor(actual, ACTIONS.ACTION_LEFT, abiertosCola, abiertos, cerrados);
            procesarSucesor(actual, ACTIONS.ACTION_DOWN, abiertosCola, abiertos, cerrados);
        }

        // Si se encontró solución, se reconstruye el plan
        // siguiendo los punteros a padres desde el objetivo
        if (nodoObjetivoEncontrado != null) {
            plan = reconstruirPlan(nodoObjetivoEncontrado);
        } else {
            plan = new ArrayList<>();
        }

        // Actualizamos el numero de abiertos y de cerrados
        nodosAbiertosFinal = abiertos.size();
        nodosCerradosFinal = cerrados.size();
    }

    /**
     * * QUÉ HACE:
     * Simula la ejecución de una acción sobre el estado actual. Si la acción es válida (no devuelve null),
     * calcula los costes (g, h, f) del nuevo nodo resultante y decide si debe incluirse en abiertos, si debe descartarse, 
     * o si debe reemplazar a un nodo previo porque se ha encontrado una ruta más óptima hacia el mismo estado.
     * * CÓMO LO HACE:
     * 1. Llama a aplicarAccion para obtener el hipotético estado sucesor.
     * 2. Si el estado es válido, instancia un nuevo Nodo y calcula su g(n) = g(actual) + 1, y su heurística.
     * 3. Consulta los HashMaps cerrados y abiertos en tiempo O(1) para verificar si este estado ya existe.
     * 4. Si el estado es nuevo, se inserta. Si ya existía 
     * (en abiertos o en cerrados) pero el nuevo camino tiene menor coste g(n), se actualizan 
     * las estructuras borrando la versión antigua e insertando la nueva.
     * * POR QUÉ:
     * Gracias a usar abiertos.get(estado) y cerrados.get(estado), 
     * se evita iterar linealmente por la PriorityQueue, resolviendo el principal cuello de botella de A*.
     * Si se encuentra un camino mejor a un estado ya cerrado, se le "rescata" de cerrados y se 
     * reinserta en abiertosCola. Esto garantiza que el algoritmo no devuelva rutas que no sean la optima.
     */
    private void procesarSucesor( Nodo actual, ACTIONS accion, PriorityQueue<Nodo> abiertosCola, HashMap<Estado, Nodo> abiertos, HashMap<Estado, Nodo> cerrados) {
        // Obtenemos el nuevo estado tras simular la acción. Null si provoca muerte o choca.
        Estado estadoSucesor = aplicarAccion(actual.estado, accion);
        if (estadoSucesor == null) return;
    
        int gNuevo = actual.g + 1; // Cada sucesor debe costar 1, tambien cuando esta volando, porque cada tick es un estado distinto
        int hNuevo = heuristica(estadoSucesor);
        int fNuevo = gNuevo + hNuevo;
    
        // Creación del nodo trazando el camino de vuelta hacia el padre
        Nodo sucesor = new Nodo(estadoSucesor, actual, accion, actual.profundidad + 1);
        sucesor.g = gNuevo;
        sucesor.h = hNuevo;
        sucesor.f = fNuevo;
        sucesor.ordenInsercion = contadorInsercion; // Para desempates
        contadorInsercion++;
        
        // Obtenemos el nodo (value) asociado con el estado (key) y null si la key no existe en el map
        // De esta manera podemos replicar el pseuocodigo para comprobar si el nodo sucesor esta ya
        // en cerrados o abiertos
        Nodo nodoEnCerrados = cerrados.get(estadoSucesor);
        Nodo nodoEnAbiertos = abiertos.get(estadoSucesor);
    
        // Si el estado sucesor tiene un nodo asociado en cerrados y el nodo sucesor tiene un menor g(n),
        // entonces es que hemos encontrado un mejor camino a un nodo ya explorado, luego lo sacamos de cerrados
        // y lo volvemos a añadir a abiertos con el mejor camino encontrado
        if (nodoEnCerrados != null && sucesor.g < nodoEnCerrados.g) {
            // Actualizamos g(n)
            cerrados.remove(estadoSucesor);
            abiertosCola.add(sucesor);
            abiertos.put(estadoSucesor, sucesor);
            return;
        }
    
        // Si no hay ningun nodo en cerrados asociado con el estado sucesor y tampoco ningun nodo en abiertos,
        // en este caso actualizamos abiertos ya que se trata de un nodo que no ha sido explorado
        if (nodoEnCerrados == null && nodoEnAbiertos == null) {
            // Nodo no visitado, lo añadimos
            abiertosCola.add(sucesor);
            abiertos.put(estadoSucesor, sucesor);
            return;
        }
    
        // Por ultimo si hay un nodo asociado al estado sucesor en abiertos y el coste g(n) del nodo sucesor
        // es menor del que hay ahora en abiertos, pues actualizamos abiertos con el nuevo nodo sucesor
        if (nodoEnAbiertos != null && sucesor.g < nodoEnAbiertos.g) {
            // Actualizamos g(n) eliminando el nodo previo e insertando uno nuevo con el mejor camino
            abiertosCola.remove(nodoEnAbiertos);
            abiertosCola.add(sucesor);
            abiertos.put(estadoSucesor, sucesor);
        }
    }

    // RECONSTRUCCIÓN DEL PLAN
    private ArrayList<ACTIONS> reconstruirPlan(Nodo objetivo) {
        ArrayList<ACTIONS> acciones = new ArrayList<>();
        Nodo actual = objetivo;

        while (actual.padre != null) {
            acciones.add(actual.accionDesdePadre);
            actual = actual.padre;
        }

        // Como el plan lo reconstruimos desde el nodo objetivo, la primera accion es la 
        // ultima que se añade, por lo que hay que invertirlo 
        Collections.reverse(acciones);
        return acciones;
    }
}
