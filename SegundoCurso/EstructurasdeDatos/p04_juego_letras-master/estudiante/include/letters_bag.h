#ifndef __LETTERS_BAG_H__
#define __LETTERS_BAG_H__

#include <stdio.h>
#include "bag.h"
#include "letters_set.h"

using namespace std;

/**
 * @brief TDA LettersBag
 *
 * Este TDA almacena un conjunto de chars utilizado en el juego de letras.
 * La estructura de datos subyacente es una lista de chars.
 */
class LettersBag {
private:
    Bag <char> letters;

public:
    /**
     * @brief Constructor por defecto
     */
    LettersBag () = default;

    /**
     * @brief Constructor dado un LettersSet
     *
     * Dado un LettersSet como argumento, este constructor debe rellenar la LettersBag con las letras que contiene el
     * LettersSet, introduciendo cada letra el número de veces indicado por el campo LetterInfo::repetitions. Un
     * ejemplo de ejecución en el que se utilice este constructor es el siguiente:
     *
     * @verbatim
        ifstream archivo_letras("letras.txt");

        LettersSet conjunto_letras;
        archivo_letras >> conjunto_letras;

        LettersBag bolsa_letras(conjunto_letras);
     @endverbatim
     *
     * @param lettersSet TDA LettersSet a parsear
     */
    LettersBag (const LettersSet & lettersSet){
        llenaBolsa(lettersSet);
    }

    /**
     * @brief Introduce una letra en la bolsa
     * @param I Letra a añadir a la LetterBag
     */
    void insertLetter (const char & I);

    /**
     * @brief Extrae una letra aleatoria de la bolsa
     *
     * Extrae una letra aleatoria de la bolsa, eliminándola del conjunto
     * @return char representa la letra extraida
     */
    char extractLetter ();

    /**
     * @brief Extrae un conjunto de letras
     *
     * Extrae un conjunto de letras de la LettersBag, eliminándolas del conjunto
     * @param num Número de letras a extraer
     * @return Lista con las letras extraídas aleatoriamente
     */
    vector<char> extractLetters (int num);


    /**
     * @brief Vacía la LettersBag
     *
     * Elimina el contenido de la LettersBag
     */
    void clear ();


    /**
     * @brief Tamaño de la bolsa
     * @return int con el tamaño de la bolsa
     */
    unsigned int size () const;


    /**
     * @brief Sobrecarga del operador de asignación
     * @param other
     * @return Referencia a this de esta forma el operador puede ser encadenado
     */
    LettersBag & operator= (const LettersBag & other);


private:
    void llenaBolsa (const LettersSet & lettersSet);
};

#endif
