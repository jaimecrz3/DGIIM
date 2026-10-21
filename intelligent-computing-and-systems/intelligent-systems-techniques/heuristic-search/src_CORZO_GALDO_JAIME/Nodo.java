package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import ontology.Types;

/**
 * Nodo de búsqueda asociado a un estado.
 *
 * A diferencia de Estado, que solo representa la configuración del mundo,
 * Nodo almacena además la información necesaria para la busqueda:
 * - enlace al nodo padre,
 * - acción aplicada desde el padre,
 * - profundidad en el árbol,
 * - coste acumulado g(n),
 * - heurística h(n),
 * - evaluación f(n) = g(n) + h(n),
 * - y orden de inserción para desempates FIFO.
 *
 * Distintos nodos pueden representar el mismo estado si se ha llegado a él
 * por caminos diferentes.
 */
public class Nodo {
    Estado estado; // Información de donde estoy y como esta el mundo
    Nodo padre; // el nodo padre para reconstruir la ruta
    Types.ACTIONS accionDesdePadre; // La acción que me llevo aqui
    int profundidad; // Profundidad del algoritmo Busqueda en profundidad
    int g; // Coste acumulado desde el estado inicial hasta este nodo: g(n)
    int h; // Valor heurístico estimado hasta el objetivo: h(n)
    int f; // Evaluación total del nodo: f(n) = g(n) + h(n)

    // Orden de inserción en abiertos. Se usa para desempatar siguiendo criterio FIFO cuando empatan f y h.
    long ordenInsercion;

    // Constructor
    // En profundidad, basta con profundidad.
    // En A*, además se completa con g, h, f y ordenInsercion
    public Nodo(Estado estado, Nodo padre, Types.ACTIONS accionDesdePadre, int profundidad) {
        this.estado = estado;
        this.padre = padre;
        this.accionDesdePadre = accionDesdePadre;
        this.profundidad = profundidad;

        this.g = 0;
        this.h = 0;
        this.f = 0;
        this.ordenInsercion = 0;
    }

    public Nodo(Estado estado, Nodo padre, Types.ACTIONS accionDesdePadre, int profundidad, int g, int h, int f, long ordenInsercion) {
        this.estado = estado;
        this.padre = padre;
        this.accionDesdePadre = accionDesdePadre;
        this.profundidad = profundidad;

        this.g = g;
        this.h = h;
        this.f = f;
        this.ordenInsercion = ordenInsercion;
    }

    public int getProfundidad(){
        return profundidad;
    } 
}