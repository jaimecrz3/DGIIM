package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.ArrayDeque;
import java.util.HashMap;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

/**
 * Clase que implementa un agente deliberativo basado en el algoritmo de búsqueda heurística en tiempo real LRTA*(k).
 * * QUÉ HACE:
 * Guía al avatar hacia el objetivo tomando decisiones de movimiento paso a paso (online) entrelazando la búsqueda y la ejecución. 
 * A diferencia de los algoritmos offline como A* (que calculan el plan completo a priori), LRTA*(k) determina 
 * la siguiente acción explorando únicamente un espacio local de búsqueda. 
 * * Además, incorpora un espacio local de aprendizaje para evitar quedar atrapado en ciclos infinitos o mínimos locales: 
 * actualiza el valor heurístico de los estados visitados a medida que descubre que el coste real es mayor al estimado inicialmente. 
 * La característica distintiva de la variante parametrizada LRTA*(k) es que propaga estos cambios heurísticos hasta un máximo de k 
 * nodos predecesores, siempre y cuando el cambio provenga de su "soporte" (su mejor vecino), lo que acelera notablemente la 
 * convergencia hacia el camino óptimo en sucesivas ejecuciones.
 * * CÓMO LO HACE:
 * En cada ciclo de decisión (tick del juego), el agente realiza los siguientes pasos:
 * 1. Generación y Evaluación: A partir del estado actual, genera los sucesores inmediatos simulando las acciones permitidas.
 * 2. Aprendizaje y Propagación (LookaheadUpdateK): Evalúa los sucesores calculando f(y) = c(x, y) + h(y). Aplica la regla 
 * de aprendizaje actualizando la heurística del estado actual con el valor del primer mínimo: h(x) = max(h(x), min(c(x,y) + h(y))). 
 * Si h(x) aumenta, el cambio se propaga a los vecinos que lo tienen como soporte (hasta k saltos iterativos usando una cola).
 * 3. Selección de Movimiento: Selecciona y ejecuta la acción que conduce al vecino con el menor coste estimado (estrategia de movimiento).
 * * POR QUÉ (Decisiones de diseño y optimización de rendimiento):
 * Dado el límite estricto de tiempo de procesamiento por tick, se han tomado las siguientes decisiones estructurales:
 * - Tablas Hash (HashMap) para Heurística y Soportes: Permiten consultar y actualizar los valores aprendidos y la relación de 
 * soportes en tiempo constante O(1), minimizando la sobrecarga computacional durante la actualización del espacio de aprendizaje.
 * - Variables de estado optimizadas (BitSet frente a boolean[][] o listas): Para representar elementos dinámicos del entorno 
 * (monedas, llaves, catapultas activas) se emplean máscaras de bits. Esto permite que la clonación y mutación del estado al 
 * simular acciones sea casi instantánea y requiera un consumo mínimo de memoria.
 * - Evaluación de sucesores "al vuelo" (On-the-fly): En lugar de instanciar y almacenar los nodos sucesores en estructuras de 
 * datos intermedias, los estados vecinos se simulan, se evalúan frente a los mejores valores registrados y se descartan o 
 * sobrescriben inmediatamente. Esto reduce drásticamente la creación de objetos, aliviando el trabajo del Garbage Collector.
 */
public class AgenteLRTAStarK extends AgenteBase {

    private static int k = 5;

    private boolean metricasImpresa; // True si se han impreso las métricas con la clase metricsProvider

    // Métricas 
    private int numAccionesPlan; // Indica el numero de acciones que ha realizado el algoritmo de busqueda
    private int nodosExpandidos; // Indica el numero de nodos a los que se les ha caculado los sucesores
    private long tiempoMilisegundos; // Tiempo en calcular el plan
    private int actualizacionesHeuristica; // Numero de actualizaciones de la heuristica

    // Guardamos el estado actual del agente, ya que la informacion que necesitamos no requiere del nodo,
    // es decir, no necesitamo saber quien es el padre para reconstruir el plan, y para la heuristica vamos
    // a usar una tablas hash. De esta manera, usamos una estructura mas ligera y ágil que si usasemos la clase Nodo. 
    private Estado actual;
    // TablaHash para guarda la heuristica de cada estado, usamos un HashMap porque para operaciones de acceder y actualizar
    // valores es una estructura de datos muy eficiente, lo que nos va a permitir facilmente añadir el valor de la heuristica
    // a un estado determinado por primera vez, haciendo uso de que Integer es un objeto y por lo tanto puede ser null (si un 
    // estado, e, todavia no esta en la tabla, tablaH.get(e) devuelve null) y actualizar ese valor cuando el algoritmo de 
    // busqueda lo requiera.
    private HashMap<Estado, Integer> tablaH;

    // TablaHash para gaurdar el soporte (mejor vecino) de cada estado, usamos un HashMap porque para operaciones de acceder y 
    // actualizar valores es una estructura de datos muy eficiente, lo que nos va a permitir facilmente añadir el soporte de 
    // un estado y consultarlo e el metodo LookaheadUpdateK.
    private HashMap<Estado, Estado> soporte;

    // Constructor
    public AgenteLRTAStarK(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // El constructor puede inicializar todas las estructuras iniciales y hasta el nodo inicial pero no puede hacer 
        // nada del proceso de búsqueda
        this.tablaH = new HashMap<>();
        this.soporte = new HashMap<>();
        this.metricasImpresa = false;

        this.numAccionesPlan = 0;
        this.nodosExpandidos = 0;
        this.tiempoMilisegundos = 0L;
        this.actualizacionesHeuristica = 0;

        this.actual = estadoInicial;
    }

    /**
     * Bucle principal del algoritmo de búsqueda LRTA*(k)
     * * QUÉ HACE:
     * Decide la siguiente acción a ejecutar en el tick actual, actualiza las métricas de evaluación, 
     * comprueba si se aplica la regla de aprendizaje de LRTA*(k) sobre el estado que se acaba de abandonar y 
     * en caso afirmativo se activa la propagacion
     * * CÓMO LO HACE:
     * 1. Delega en lookaheadUpdateK() la fase de aprendizaje para actualizar la heurística.
     * 2. Evalúa todos los estados sucesores posibles desde el estado actual
     * 4. Selecciona el sucesor que minimice el coste f(y) = 1 + h(y). En caso de no haber
     * salida, devuelve ACTION_NIL. Si la hay, actualiza su estado actual y devuelve la acción.
     * * POR QUÉ:
     * Al evaluar solo a los vecinos inmediatos y tomar decisiones paso a paso, permite al agente reaccionar 
     * en tiempo real, salir de zonas estancadas y dirigir sus pasos hacia el portal progresivamente.
     */
    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        if (stateObs.getGameTick() >= 5000) {
            // Si se llega al tiempo maximo de juego y no se ha conseguido el ojetivo, 
            // mostamos las metricas ya que se pierde la partida
            fijarMetricasYMostrar();
        }
        long tInicio = System.nanoTime();

        if (esObjetivo(actual)) { // Si estamos en el portal con la llave acabamos
            long tFin = System.nanoTime();
            tiempoMilisegundos += tFin - tInicio;
            fijarMetricasYMostrar();
            return ACTIONS.ACTION_NIL;
        }

        // En LRTA*(k) se expande un nodo y se toma una acción por cada tick invariablemente
        nodosExpandidos++;
        numAccionesPlan++;

        // Calculamos la heuristica y la propaamos en hasta k nodos
        lookaheadUpdateK(actual, k);

        // Ahora vamos aseleccionar e mejor sucesor/vecino
        //
        // Aqui vamos a almacenar toda la informacion relacionada con el sigiente movimiento
        // mejorEstado va a ser el estado hacia el que se mueve el agente, y null si muere 
        Estado mejorEstado = null;
        // mejorAccion va a ser la accion con la que llega a mejorEstado
        ACTIONS mejorAccion = ACTIONS.ACTION_NIL;
        // mejorValor va a ser el min de los c(x,y) + h(y) donde x es el nodo actual e y son sus
        // posibles vecinos
        int mejorValor = 0;

        // Disitinguimos entre que el agente este volando, transformandose entre vampiro y murcielago
        // y cambiando de direccion en una catapulta, en cuyo caso la unica accion a expandir es ACTION_NIL,
        // y el resto de casos que se expande en el orden: DEREHA, ARRIBA, IZQUIERDA, ABAJO 
        if (actual.enVuelo || actual.pendienteTransformacion) {
            Estado suc = aplicarAccion(actual, ACTIONS.ACTION_NIL);
            if (suc == null) { // Si muere terminamos y mostramos las metricas
                long tFin = System.nanoTime();
                tiempoMilisegundos += tFin - tInicio;
                fijarMetricasYMostrar();
                return ACTIONS.ACTION_NIL;
            }

            // Si la accion es valida, calculamos el coste del desplazamiento e indicamos
            // quien es el candidato, que en este caso, al no haber mas posibles movimientos, es el elegido
            int valor = 1 + obtenerH(suc);
            mejorValor = valor;
            mejorEstado = suc;
            mejorAccion = ACTIONS.ACTION_NIL;
        } else { // Si va andando normal

            for (ACTIONS accion : ORDEN_EXPANSION) { // ORDEN_EXPANSION = DERECHA, ARRIBA, IZQUIERDA, ABAJO
                Estado suc = aplicarAccion(actual, accion);
                if (suc == null) continue; // Pasamos a la siguiente accion, ya que esta no era valida

                int valor = 1 + obtenerH(suc); // Coste del nuevo estado
                if (mejorEstado == null || valor < mejorValor) { 
                    // Si es la primera accion valida, inicializamos y posteriormente si obtenemos un vecino con menor coste actualizamos 
                    mejorValor = valor;
                    mejorEstado = suc;
                    mejorAccion = accion;
                } 
            }
        }

        if (mejorEstado == null) { // Si no hay accion valida terminamos
            long tFin = System.nanoTime();
            tiempoMilisegundos += tFin - tInicio;
            fijarMetricasYMostrar();
            return ACTIONS.ACTION_NIL;
        }

        actual = mejorEstado; // Actualizamos el estado actual 

        long tFin = System.nanoTime();
        tiempoMilisegundos += tFin - tInicio;

        // Comprobamos si con la nueva accion estamos en el portal con la llave
        if (esObjetivo(actual)) {
            fijarMetricasYMostrar();
        }

        return mejorAccion;
    }

    /**
     * * QUÉ HACE:
     * Actualiza el valor heurístico del estado actual basándose en su mejor sucesor y, 
     * si este valor sube, propaga el cambio hacia los vecinos que le tengan como soporte.
     * * CÓMO LO HACE:
     * Emplea una cola partiendo del estado actual y repite iterativamente (hasta k veces):
     * 1. Extrae un estado x y localiza a su mejor vecino (mejorEstado).
     * 2. Guarda a ese vecino como el soporte de x.
     * 3. Aplica la regla de aprendizaje de LRTA*: si el coste real de moverse al mejor vecino 
     * (1 + h(vecino)) es mayor que la h(x) actual, actualiza h(x).
     * 4. Si h(x) aumentó, busca qué estados anteriores tenian como soporte x 
     * y los añade a la cola para intentar actualizar su heuristica.
     * * POR QUÉ:
     * Al propagar k pasos hacia atrás acelera la convergencia del algoritmo,
     * ya que avisa al camino previo de que la ruta elegida era peor de lo esperado.
     */
    private void lookaheadUpdateK(Estado estadoActual, int k){
        // Cola para almacenar el nodo actual y las posibles k propagaciones
        ArrayDeque<Estado> cola = new ArrayDeque<>(); 
        cola.add(estadoActual);
        
        int contador = k-1;

        while(!cola.isEmpty()){ // Mientras que haya elementos en la cola
            Estado x = cola.poll(); // Extraemos el primer elemento de la cola

            // CALCULO DEL MEJOR VECINO DE X
            // Aqui vamos a almacenar toda la informacion relacionada con el sigiente movimiento
            // mejorEstado va a ser el estado hacia el que se mueve el agente, y null si muere 
            Estado mejorEstado = null;
            // mejorValor va a ser el min de los c(x,y) + h(y) donde x es el nodo actual e y son sus
            // posibles vecinos
            int mejorValor = 0;

            // Disitinguimos entre que el agente este volando, transformandose entre vampiro y murcielago
            // y cambiando de direccion en una catapulta, en cuyo caso la unica accion a expandir es ACTION_NIL,
            // y el resto de casos que se expande en el orden: DEREHA, ARRIBA, IZQUIERDA, ABAJO 
            if (x.enVuelo || x.pendienteTransformacion) {
                Estado suc = aplicarAccion(x, ACTIONS.ACTION_NIL);
                if (suc == null) { // Si la accion no es valida, actualizamos el soporte y salimos dirctamente del bucle
                    soporte.put(x, null);
                    continue;
                }

                // Si la accion es valida, calculamos el coste del desplazamiento e indicamos
                // quien es el candidato, que en este caso, al no haber mas posibles movimientos, es el elegido
                int valor = 1 + obtenerH(suc);
                mejorValor = valor;
                mejorEstado = suc;
            } else { // Si va andando normal

                for (ACTIONS accion : ORDEN_EXPANSION) { // ORDEN_EXPANSION = DERECHA, ARRIBA, IZQUIERDA, ABAJO
                    Estado suc = aplicarAccion(x, accion);
                    if (suc == null) continue; // Pasamos a la siguiente accion, ya que esta no era valida

                    int valor = 1 + obtenerH(suc); // Coste del nuevo estado
                    if (mejorEstado == null || valor < mejorValor) { 
                        // Si es la primera accion valida, inicializamos y posteriormente si obtenemos un vecino con menor coste actualizamos 
                        mejorValor = valor;
                        mejorEstado = suc;
                    } 
                }
            }

            if (mejorEstado == null) { // Si no habia ninguna accion valida, actualizamos el soporte y salimos dirctamente del bucle
                soporte.put(x, null);
                continue;
            }

            // Añadimos el mejor vecino como soporte del nodo
            soporte.put(x, mejorEstado);

            // ACTALIZACION DE H(X)
            boolean propagar = false;
            int hVieja = obtenerH(x);
            int hNueva = mejorValor;
            if (hVieja < hNueva) { // Si se cumple actualizamos segun la regla de aprendizaje de LRTA*(k)
                propagar = true;
                actualizacionesHeuristica++;
                tablaH.put(x, hNueva);
            }

            // PROPAGACION
            if (propagar) { // Como se ha actualiado a heuristica, estamos en el caso enel que se puede propagar los cambios huristicos
                // Vamos a recorrer los sucesores y mientras no lleguemos a k propagaciones y el mejor vecino del sucesor del nodo
                // sea el propio nodo, lo añadimos a la cola para que se pueda actualizar su informacio heuristica

                // Disitinguimos entre que el agente este volando, transformandose entre vampiro y murcielago
                // y cambiando de direccion en una catapulta, en cuyo caso la unica accion a expandir es ACTION_NIL,
                // y el resto de casos que se expande en el orden: DEREHA, ARRIBA, IZQUIERDA, ABAJO 
                if (x.enVuelo || x.pendienteTransformacion) {
                    Estado suc = aplicarAccion(x, ACTIONS.ACTION_NIL);
                    Estado soporteSuc = null;
                    if (suc != null) {
                        soporteSuc = soporte.get(suc);
                    }
                    if (soporteSuc != null && contador > 0 && soporteSuc.equals(x)) {
                        cola.add(suc);
                        contador--;
                    }
                } else { // Si va andando normal

                    for (ACTIONS accion : ORDEN_EXPANSION) { // ORDEN_EXPANSION = DERECHA, ARRIBA, IZQUIERDA, ABAJO
                        Estado suc = aplicarAccion(x, accion);
                        Estado soporteSuc = null;
                        if (suc != null) {
                            soporteSuc = soporte.get(suc);
                        }
                        if (soporteSuc != null && contador > 0 && soporteSuc.equals(x)) {
                            cola.add(suc);
                            contador--;
                        }
                    }
                }
            } // if (propagar)
        } // while(!cola.isEmpty())
    }

    // Este metodo proporciona el coste estimado desde un estado especifico hasta la meta, que se ha
    // podido ir actualizando con la regla de aprendizaje del algoritmo de busqueda. 
    // Para proporcionar el coste devuelve el valor asociado al estado que es la llave de la tabla
    // hash. Si todavia no existe esa llave, se calcula la heuristica inicial usando la distancia
    // Manhattan hasta el portal y lo guarda en la tabla.
    private int obtenerH(Estado e) {
        Integer h = tablaH.get(e);
        if (h == null) {
            h = Math.abs(e.x - portalX) + Math.abs(e.y - portalY);
            tablaH.put(e, h);
        }
        return h;
    }

    private void fijarMetricasYMostrar() {
        if (metricasImpresa) return;
        System.out.println("Tamaño de la tabla hash: " + tablaH.size());
        MetricsProvider.getInstance().setAgente("AgenteLRTAStarK");
        MetricsProvider.getInstance().setNumAccionesPlan(numAccionesPlan);
        MetricsProvider.getInstance().setNodosExpandidos(nodosExpandidos);
        MetricsProvider.getInstance().setNumActualizacionesTabla(actualizacionesHeuristica);
        MetricsProvider.getInstance().setTiempoMilisegundos(tiempoMilisegundos / 1_000_000L);
        MetricsProvider.getInstance().printMetrics();
    
        metricasImpresa = true;
    }
}
