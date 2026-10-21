#ifndef P04_JUEGO_LETRAS_SOLVER_H
#define P04_JUEGO_LETRAS_SOLVER_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "dictionary.h"
#include "letters_set.h"
#include "letters_bag.h"

/**
 * @brief TDA Solver
 * @details Almacena un diccionario de palabras permitidas,
 * y un LettersSet con la información de las letras para poder formar sus soluciones
 *
 */

class Solver {
private:
    Dictionary diccionario;
    LettersSet letras;

public:
    /**
     * @brief Constructor por defecto
     */
    Solver() = default;

    /**
     * @brief Constructor de copia
     * @details Crea un Solver con el mismo contenido que el que se pasa como argumento
     * @param other Solver que se quiere copiar
     */

    Solver (const Solver & other);

    /**
     * @brief Constructor con argumentos
     * @details Crea un Solver que tiene asociado un Dictionary y un LettersSet
     * @param dic Dictionary asociado
     * @param letters_set LettersSet asociado
     */

    Solver (const Dictionary &dic, const LettersSet &letters_set);

    /**
     * @brief Construye el vector de soluciones a partir de las letras de una partida
     * Dado un conjunto de letras posibles para crear una solucion, y el modo de juego con el que se juega la partida,
     * se construye un vector con las mejores soluciones encontradas en funcion del tipo de juego planteado
     * @param available_letters vector de letras disponibles para la partida
     * @param score_game Bool indicando en tipo de la partida, true indica que la partida se juega a puntuacion, false que se juega a longitud
     * @return El vector de palabras con las mejores soluciones, y su puntuación
     */

    pair<vector<string>, int> getSolutions(const vector<char> &available_letters, bool score_game);

    /**
    * @brief El resultado obtenido es el mismo que con getSolutions pero de forma mas eficiente
    * usando el iterador de palabras válidas
    * @param available_letters vector de letras disponibles para la partida
    * @param score_game Bool indicando en tipo de la partida, true indica que la partida se juega a puntuacion, false que se juega a longitud
    * @return El vector de palabras con las mejores soluciones, y su puntuación
    */
    pair<vector<string>, int> getSolutionsEficiente(const vector<char> &available_letters, bool score_game);

private:
    pair<vector<string>, int> getSolutionsScore(const vector<char> &available_letters);

    pair<vector<string>, int> getSolutionsLength(const vector<char> &available_letters);

    pair<vector<string>, int> getSolutionsScoreEficiente(const vector<char> &available_letters);

    pair<vector<string>, int> getSolutionsLengthEficiente(const vector<char> &available_letters);

    bool esValida (string palabra, vector<char> &available_letters);

};



#endif //P04_JUEGO_LETRAS_SOLVER_H
