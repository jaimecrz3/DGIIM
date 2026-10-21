package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

/**
 * Clase que modeliza el algoritmo de búsqueda en profundidad (Depth-First Search).
 * * QUÉ HACE:
 * Calcula el plan de acciones secuencial que debe ejecutar el agente (vampiro/murciélago) 
 * para alcanzar la meta. Al ser un algoritmo de búsqueda no informada (ciega), explora 
 * el espacio de estados de forma exhaustiva sin utilizar información heurística sobre 
 * la distancia al objetivo.
 * * CÓMO LO HACE:
 * Utiliza una representación basada en Nodos (que guardan la traza y profundidad de la búsqueda) 
 * y Estados (que representan la configuración del mundo en un momento dado). Implementa 
 * el algoritmo DFS de forma recursiva. A diferencia del pseudocódigo clásico, no genera 
 * ni almacena la lista completa de sucesores en cada paso; en su lugar, genera, valida y 
 * explora cada sucesor directamente en el orden establecido. Utiliza un conjunto `HashSet` 
 * para el registro de estados explorados y objetos `BitSet` para monitorizar los elementos 
 * dinámicos del mapa (monedas, llaves y catapultas).
 * * POR QUÉ (Decisiones de diseño y optimización):
 * - HashSet frente a ArrayList: En DFS, el registro de estados visitados crece exponencialmente. 
 * Verificar duplicados en un ArrayList requiere un tiempo de O(N), creando un cuello de botella. 
 * El HashSet, apoyado en un cálculo eficiente del hashCode, permite búsquedas e inserciones 
 * en tiempo constante O(1).
 * - BitSet frente a boolean[][]: Representar la existencia de elementos dinámicos con matrices 
 * bidimensionales resultaba prohibitivo en mapas grandes, tanto en consumo de memoria como 
 * en el tiempo necesario para clonarlas en cada nuevo estado. `BitSet` comprime esta información 
 * a nivel de bits, haciendo que las operaciones de copiado mutación sean drásticamente más rápidas.
 * - Generación de sucesores "al vuelo": Evaluar las acciones directamente sin instanciar 
 * una lista temporal de sucesores reduce significativamente la generación de "basura" en 
 * memoria (Garbage Collection), acelerando el tiempo total de ejecución.
 */
public class AgenteProfundidad extends AgenteBase {

    // Plan
    private ArrayList<ACTIONS> plan;
    // índice de la siguiente acción del plan que se devuelve en act() con la funcion calcularProfundidad() 
    private int indicePlan;  
    private boolean planCalculado; // True una vez que se ha calculado el plan con la funcion calcularProfundidad() 
    private boolean metricasImpresa; // True si se han impreso las métricas con la clase metricsProvider

    // Métricas 
    private int nodosExpandidos; // Indica el numero de nodos a los que se les ha caculado los sucesores
    private int profundidadMaxima; // Cada nodo tiene una profundidad en el arbol de busqueda, aqui almacenamos la maxima
    private long tiempoMilisegundos; // Tiempo en calcular el plan

    // Resultado de la búsqueda 
    private Nodo nodoObjetivoEncontrado;

    // Constructor
    public AgenteProfundidad(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // El constructor puede inicializar todas las estructuras iniciales y hasta el nodo inicial pero no puede hacer 
        // nada del proceso de búsqueda
        this.plan = new ArrayList<>();
        this.indicePlan = 0;
        this.planCalculado = false;
        this.metricasImpresa = false;

        this.nodosExpandidos = 0;
        this.profundidadMaxima = 0;
        this.tiempoMilisegundos = 0;
        this.nodoObjetivoEncontrado = null;

        //imprimirTipos(stateObs);
        
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

            calcularPlanProfundidad();

            long tFin = System.nanoTime();
            tiempoMilisegundos = (tFin - tInicio) / 1_000_000L;

            planCalculado = true;

            // Al encontrar el plan se deben fijar TODAS las métricas complementarias correspondientes al algoritmo
            MetricsProvider.getInstance().setAgente("AgenteProfundidad");
            MetricsProvider.getInstance().setNumAccionesPlan(plan.size());
            MetricsProvider.getInstance().setNodosExpandidos(nodosExpandidos);
            MetricsProvider.getInstance().setProfundidadMaxima(profundidadMaxima);
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

    // BÚSQUEDA EN PROFUNDIDAD 
    // Lanza la búsqueda DFS desde el estado inicial.
    // Si encuentra objetivo, reconstruye el plan completo de acciones.
    private void calcularPlanProfundidad() {
        Nodo nodoInicial = new Nodo(estadoInicial, null, ACTIONS.ACTION_NIL, 0);

        // QUÉ: Estructura para almacenar los estados ya explorados y evitar ciclos infinitos.
        // CÓMO: Se utiliza un HashSet en lugar de un ArrayList o List tradicional.
        // POR QUÉ: En un algoritmo DFS, la lista de visitados crece exponencialmente. 
        // Un ArrayList tiene una complejidad de O(N) para el método .contains(), lo que 
        // crea un cuello de botella. HashSet utiliza una tabla hash (mediante hashCode), 
        // permitiendo búsquedas e inserciones en tiempo constante O(1).
        Set<Estado> visitados = new HashSet<>();
        visitados.add(estadoInicial);

        boolean encontrado = dfs(nodoInicial, visitados);

        // Si se encontró solución, se reconstruye el plan
        // siguiendo los punteros a padres desde el objetivo
        if (encontrado && nodoObjetivoEncontrado != null) {
            this.plan = reconstruirPlan(nodoObjetivoEncontrado);
        } else {
            this.plan = new ArrayList<>(); // por seguridad
        }
    }

    private boolean dfs(Nodo actual, Set<Estado> visitados) {
        // Actualizamos la profundidad máxima alcanzada
        if (actual.profundidad > profundidadMaxima) {
            profundidadMaxima = actual.profundidad;
        }

        // Si el estado actual ya es objetivo, terminamos.
        // No lo contamos como expandido ya que no se generan sus sucesores 
        if (esObjetivo(actual.estado)) {
            nodoObjetivoEncontrado = actual;
            return true;
        }

        nodosExpandidos++;

        // GENERACIÓN DE SUCESORES 
        // Genera los sucesores del nodo actual respetando:
        // 1) el orden de expansión Derecha, Arriba, Izquierda, Abajo,
        // 2) Desde que entra un catapulta hasta la tranfomacion de vuelta a vampiro solo se usa ACTION_NIL
        //
        // Procesamos cada accion directamente, para ahorrarnos tener que crear una lista de sucesores sobre la
        // que iterar, de esta manera, reducimos el numero de basura y llamadas
        if (actual.estado.enVuelo || actual.estado.pendienteTransformacion) {
            return probarSucesor(actual, ACTIONS.ACTION_NIL, visitados);
        }
    
        return probarSucesor(actual, ACTIONS.ACTION_RIGHT, visitados)
            || probarSucesor(actual, ACTIONS.ACTION_UP, visitados)
            || probarSucesor(actual, ACTIONS.ACTION_LEFT, visitados)
            || probarSucesor(actual, ACTIONS.ACTION_DOWN, visitados);
    }

    // Aunque pueda parecer una implementacion alternativa del pseudocodigo proporcionado,
    // la idea es la misma ya que para cada uno de los sucesores (que son los nodos validos resultado de aplicar cada
    // una de las acciones posibles), se comprueba que no se haya visitado previamente, y si es así,
    // se añade a visitados, se establece el padre (que en este caso es el nodo actual ) al construir el nuevo nodo
    // y se aplica la busqueda en profundidad de manera recursiva.
    //
    // Como se explica en el metodo anterior dfs, con esta implementacion se consigue no tener que crear una lista de sucesores
    // para cada nodo explorado reduciendo el numero de basura y llamadas
    private boolean probarSucesor(Nodo actual, ACTIONS accion, Set<Estado> visitados) {
        Estado siguiente = aplicarAccion(actual.estado, accion);
        if (siguiente == null) return false;
        if (visitados.contains(siguiente)) return false;

        visitados.add(siguiente);
        Nodo hijo = new Nodo(siguiente, actual, accion, actual.profundidad + 1);
        return dfs(hijo, visitados);
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

    // private void imprimirTipos(StateObservation stateObs) {
    //     imprimirGrupo("RESOURCES", stateObs.getResourcesPositions());
    //     imprimirGrupo("IMMOVABLES", stateObs.getImmovablePositions());
    //     imprimirGrupo("PORTALS", stateObs.getPortalsPositions());
    // }
    
    // private void imprimirGrupo(String nombre, ArrayList<Observation>[] grupos) {
    //     if (grupos == null) return;
    
    //     for (ArrayList<Observation> grupo : grupos) {
    //         if (grupo == null || grupo.isEmpty()) continue;
    
    //         Observation o = grupo.get(0);
    //         int gx = (int)(o.position.x / blockSize);
    //         int gy = (int)(o.position.y / blockSize);
    
    //         System.out.println(
    //             nombre +
    //             " -> category=" + o.category +
    //             ", itype=" + o.itype +
    //             ", ejemplo=(" + gx + "," + gy + ")" +
    //             ", cantidad=" + grupo.size()
    //         );
    //     }
    // }
    
}


