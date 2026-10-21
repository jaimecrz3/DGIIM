package tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME;

import java.util.Objects;
import java.util.BitSet;

/**
 * Representa un estado completo del juego en un tick concreto.
 *
 * Descripcion abstracta de la configuracion del mundo sin depender del motor de GVGAI.
 *
 * Incluye:
 * - la posición actual del avatar,
 * - el inventario (llave y monedas),
 * - si el avatar está volando o no,
 * - la dirección de vuelo en caso de estar en una catapulta,
 * - si hay una transformación pendiente,
 * - y qué objetos dinámicos siguen existiendo en el mapa
 *   (monedas, llaves y catapultas restantes).
 */
public class Estado {
    // posicion del avatar en el grid
    int x;
    int y;

    boolean tieneLlave; // si tiene la llave
    int monedas; // cuantas monedas lleva

    boolean enVuelo; // vampiro o murciélago
    int dirVuelo; // direcion si está volando, 0: east,1: south, 2: north, 3: west
    // Indica si en el siguiente tick debe producirse una transformación
    // asociada al uso de una catapulta o a un cambio de dirección en vuelo.
    boolean pendienteTransformacion; 

    // Elementos que cambian durante la partida:
    // QUÉ: Estructuras para almacenar los objetos dinámicos del mapa.
    // CÓMO: Usamos BitSet, donde cada bit representa si un objeto específico 
    // (identificado por un índice entero único precalculado) sigue existiendo o no.
    // POR QUÉ: Es mucho más eficiente en memoria que usar matrices boolean[][]. 
    // Además, el método .clone() de BitSet es mucho más rápido que hacer una copia 
    // profunda de una matriz 2D, y sus métodos internos equals() y hashCode() 
    // están altamente optimizados a nivel de bits.
    //
    // Indica qué monedas siguen disponibles en el mapa. 
    BitSet monedasRestantes;
    // Indica qué catapultas siguen disponibles en el mapa.
    BitSet catapultasRestantes;
    // Indica qué llaves siguen disponibles en el mapa
    BitSet llavesRestantes;  

    // Se invoca solo cuando dos objetos comparten el mismo hashCode (colisión) para determinar si son realmente el mismo estado o uno diferente.
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof Estado)) return false;
        Estado b = (Estado) obj;

        return x == b.x &&
            y == b.y &&
            tieneLlave == b.tieneLlave &&
            monedas == b.monedas &&
            enVuelo == b.enVuelo &&
            dirVuelo == b.dirVuelo &&
            pendienteTransformacion == b.pendienteTransformacion &&
            Objects.equals(monedasRestantes, b.monedasRestantes) &&
            Objects.equals(catapultasRestantes, b.catapultasRestantes) &&
            Objects.equals(llavesRestantes, b.llavesRestantes);
    }

    // Genera un entero que permite a HashSet ubicar rápidamente el estado en una posición específica de la tabla hash
    @Override
    public int hashCode() {
        return Objects.hash(x, y, tieneLlave, monedas, enVuelo, dirVuelo, pendienteTransformacion,
                                monedasRestantes, catapultasRestantes, llavesRestantes);

    }

    // Constructor vacio
    public Estado() {
    }

}