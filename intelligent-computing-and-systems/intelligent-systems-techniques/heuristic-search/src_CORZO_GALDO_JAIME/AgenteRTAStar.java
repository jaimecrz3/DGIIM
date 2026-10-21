package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.HashMap;

import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tracks.singlePlayer.MetricsProvider;

// Nota: Se puede heredar de otras clases personalizadas por el alumnado que hereden de AbstractPlayer para generalizar
// los elementos comunes a todos los algoritmos.

/**
 * Clase que implementa el agente basado en el algoritmo de búsqueda heurística en tiempo real RTA* (Real-Time A*).
 * * * QUÉ HACE:
 * Guía al avatar hacia el objetivo tomando decisiones paso a paso (online). A diferencia de 
 * algoritmos offline como A* (que calculan todo el plan antes de ejecutar el primer movimiento), 
 * RTA* calcula solo la siguiente acción basándose en un espacio local de búsqueda. Además, 
 * incorpora un espacio local de aprendizaje: actualiza la heurística de los estados visitados 
 * para rellenar "valles" de error, escapar de mínimos locales y evitar bucles infinitos.
 * * * CÓMO LO HACE:
 * En cada iteración (tick del juego), el agente genera los sucesores inmediatos del estado actual.
 * Evalúa cada sucesor sumando el coste del paso y su valor heurístico (calculado inicialmente 
 * mediante la distancia Manhattan al objetivo). Selecciona el sucesor con el menor coste total y 
 * actualiza el valor heurístico del estado actual asignándole el valor del segundo mejor 
 * sucesor (regla de aprendizaje de RTA*). Los valores heurísticos actualizados se almacenan y 
 * recuperan utilizando una tabla Hash (HashMap).
 * * * POR QUÉ (Decisiones de diseño y optimización para el rendimiento):
 * - Uso de HashMap para la heurística: Permite consultar y actualizar los valores aprendidos de 
 * los estados en tiempo constante O(1), minimizando la sobrecarga computacional durante la 
 * búsqueda.
 * - Variables de estado con BitSet (frente a boolean[][] o listas): El entorno requiere saber 
 * qué monedas, llaves y catapultas siguen activas. Representar esto con matrices implicaría 
 * un alto coste al clonar el estado para simular cada acción. BitSet comprime esta información 
 * a nivel de bits, permitiendo copias y mutaciones casi instantáneas con un consumo de memoria mínimo.
 * - Evaluación de sucesores "al vuelo": En lugar de instanciar y almacenar temporalmente los 
 * nodos sucesores en estructuras de datos complejas (como listas o colas), los vecinos se 
 * simulan, se evalúan frente a los mejores valores registrados y se descartan inmediatamente. 
 * Esto reduce drásticamente la creación de objetos innecesarios en memoria, aliviando el 
 * trabajo del recolector de basura (Garbage Collector) y acelerando el tiempo total de ejecución.
 */
public class AgenteRTAStar extends AgenteBase {

    private boolean metricasImpresa; // True si se han impreso las métricas con la clase metricsProvider

    // Métricas 
    private int numAccionesPlan; // Indica el numero de acciones que ha realizado el algoritmo de busqueda
    private int nodosExpandidos; // Indica el numero de nodos a los que se les ha caculado los sucesores
    private long tiempoMilisegundos; // Tiempo en calcular el plan

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

    // Constructor
    public AgenteRTAStar(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        super(stateObs, elapsedTimer);

        // El constructor puede inicializar todas las estructuras iniciales y hasta el nodo inicial pero no puede hacer 
        // nada del proceso de búsqueda
        this.tablaH = new HashMap<>();
        this.metricasImpresa = false;

        this.numAccionesPlan = 0;
        this.nodosExpandidos = 0;
        this.tiempoMilisegundos = 0L;

        this.actual = estadoInicial;
    }

    /**
     * Bucle principal del algoritmo de búsqueda RTA*
     * * QUÉ HACE:
     * Decide la siguiente acción a ejecutar en el tick actual, actualiza las métricas de evaluación 
     * y aplica la regla de aprendizaje de RTA* sobre el estado que se acaba de abandonar.
     * * CÓMO LO HACE:
     * 1. Incrementa los contadores de nodos expandidos y acciones del plan al inicio.
     * 2. Si el avatar está en vuelo o transformándose, la única acción evaluada es ACTION_NIL.
     * 3. Si es un vampiro normal, simula las 4 acciones posibles (DERECHA, ARRIBA, IZQUIERDA, ABAJO).
     * 4. Identifica el sucesor con el menor coste y el segundo menor coste.
     * 5. Si el agente no tiene movimientos válidos, finaliza la ejecución y muestra las métricas.
     * 6. Actualiza la tabla Hash para el estado actual con el máximo entre su 
     * heurística original y la del segundo mejor sucesor.
     * 7. Avanza al mejor estado, comprueba si es el portal y devuelve la acción elegida.
     * * POR QUÉ:
     * Se usan variables locales para la busqueda: mejorEstado, mejorAccion, mejorValor para evitar 
     * tener que usar una extructura auxiliar que aumentaria el coste de la memoria.
     * -Importante la inicializacion del segundoMin a null despues de la primera accion valida (en 
     * el caso que este andando normal), ya que de esta manera, nos permite luego actualizarlo al
     * segundo mejor valor (el segundo minimo). 
     */
    @Override
    public ACTIONS act(StateObservation stateObs, ElapsedCpuTimer elapsedTimer) {
        if (stateObs.getGameTick() >= 5000) { 
            // Si se llega al tiempo maximo de juego y no se ha conseguido el ojetivo, 
            // mostamos las metricas ya que se pierde la partida
            fijarMetricasYMostrar();
        }
        long tInicio = System.nanoTime();

        // En RTA* se expande un nodo y se toma una acción por cada tick invariablemente
        nodosExpandidos++;
        numAccionesPlan++;

        // Aqui vamos a almacenar toda la informacion relacionada con el sigiente movimiento
        // mejorEstado va a ser el estado hacia el que se mueve el agente, y null si muere 
        Estado mejorEstado = null;
        // mejorAccion va a ser la accion con la que llega a mejorEstado
        ACTIONS mejorAccion = ACTIONS.ACTION_NIL;
        // mejorValor va a ser el min de los c(x,y) + h(y) donde x es el nodo actual e y son sus
        // posibles vecinos
        int mejorValor = 0;
        // segundo_min va a ser el segundo valor mas pequeño entre los c(x,y) + h(y) donde x 
        // es el nodo actual e y son sus posibles vecinos. Se va a usar para actualizar la heuristica
        // del nodo actual: h(actual) = max(h(actual), segundoMin)
        Integer segundoMin = null;

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
            segundoMin = mejorValor; // si solo hay uno, usamos el primero
        } else { // Si va andando normal

            for (ACTIONS accion : ORDEN_EXPANSION) { // ORDEN_EXPANSION = DERECHA, ARRIBA, IZQUIERDA, ABAJO
                Estado suc = aplicarAccion(actual, accion);
                if (suc == null) continue; // Pasamos a la siguiente accion, ya que esta no era valida

                int valor = 1 + obtenerH(suc); // Coste del nuevo estado
                if (mejorEstado == null) { // Si es la primera accion valida, inicializamos 
                    mejorValor = valor;
                    segundoMin = null;
                    mejorEstado = suc;
                    mejorAccion = accion;
                } else if (valor < mejorValor) { // Ponemos menor estrictor para respetar el orden de expansion de sucesores
                    // Si econtramos una accion con un menor coste que la ya guardada, actualizamos y ahora si,
                    // inicializamos segundoMin con el mejorValor previo
                    segundoMin = mejorValor;
                    mejorEstado = suc;
                    mejorAccion = accion;
                    mejorValor = valor;
                } else if (segundoMin == null || valor < segundoMin) { 
                    // En el caso de que previamente se actualizen los valores por la primera accion valida,
                    // y luego las acciones siguientes tengan un coste mayor, por lo que no se actualizaria segundoMin,
                    // entra aqui y toma ese valor, si luego vuelva a salir un valor que no es el minimo pero es 
                    // menor que el segundoMin, tenemos que actualizar segundoMin con ese valor para que siga siendo
                    // el segundo menor coste.
                    // Ejemplo, si DERECHA = 5, IZQUIERDA = 10 y ABAJO = 8, con DERECHA entra en la primera condicion, con 
                    // IZQUIERDA entra en este condicion y se actualiza el segundoMin, y con ABAJO vuelve a entrar en esta
                    // condicion y se vuelve a actualizar el segundoMin.
                    segundoMin = valor;
                }
            }
        }

        if (mejorEstado == null) { // Si no hay accion valida terminamos
            long tFin = System.nanoTime();
            tiempoMilisegundos += tFin - tInicio;
            fijarMetricasYMostrar();
            return ACTIONS.ACTION_NIL;
        }

        if (segundoMin == null) { // En el caso de que solo hubiese una accion valida, el segundoMin es el minimo
            segundoMin = mejorValor;
        }

        // Regla de aprendizaje de RTA*
        int hActual = obtenerH(actual);
        int nuevoH = Math.max(hActual, segundoMin);
        tablaH.put(actual, nuevoH);

        actual = mejorEstado; // Actualizamos el estado actual 

        long tFin = System.nanoTime();
        tiempoMilisegundos += tFin - tInicio;

        // Comprobamos si con la nueva accion estamos en el portal con la llave
        if (esObjetivo(actual)) {
            fijarMetricasYMostrar();
        }

        return mejorAccion;
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
        MetricsProvider.getInstance().setAgente("AgenteRTAStar");
        MetricsProvider.getInstance().setNumAccionesPlan(numAccionesPlan);
        MetricsProvider.getInstance().setNodosExpandidos(nodosExpandidos);
        MetricsProvider.getInstance().setTiempoMilisegundos(tiempoMilisegundos / 1_000_000L);
        MetricsProvider.getInstance().printMetrics();
    
        metricasImpresa = true;
    }
}
