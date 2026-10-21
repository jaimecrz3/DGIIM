#ifndef __LETTER_SET_H__
#define __LETTER_SET_H__

#include <iostream>
#include <cstdlib>
#include <map>
#include <iomanip>

using namespace std;

/**
 * @brief TDA LetterInfo
 *
 * Contiene información sobre un determinado carácter del juego de las
 * letras. En concreto, almacena información sobre el número de repeticiones de
 * la letra en la partida y de la puntuación que otorga al utilizarse en una
 * palabra
 */
 struct LetterInfo {
     int repetitions;
     int score;
 };

/**
 * @brief TDA LettersSet
 *
 * Este TDA representa un conjunto de letras, con la información necesaria para
 * jugar una partida al juego de las letras, es decir, el número de repeticiones
 * que tenemos de la letra y la puntuación que dicha letra otorga cuando se
 * utiliza en una palabra
 */
class LettersSet{
private:
    map <char, LetterInfo> letters;

public:

    /**
     * @brief Constructor por defecto
     *
     * Crea un LettersSet vacío
     */
    LettersSet();

    /**
     * @brief Constructor de copia
     * @param other LettersSet a copiar
     */
    LettersSet(const LettersSet & other);

    /**
     * Inserta un elemento en el LettersSet
     * @param val Pareja de letra y LetterInfo asociada a insertar
     * @return  Booleano que marca si se ha podido insertar la letra en el LettersSet.
     *          La letra sólo se inserta correctamente si no estaba aún incluida en la colección
     */
    bool insert (const pair<char, LetterInfo> & val);

    /**
     * @brief Elimina un carácter del LettersSet
     * @param key Carácter a eliminar
     * @return Booleano que indica si se ha podido eliminar correctamente la letra del LettersSet
     */
    bool erase (const char & key);

    /**
     * @brief Limpia el contenido del LettersSet
     *
     * Elimina el contenido del LettersSet
     */
    void clear ();

    /**
     * @brief Consulta si el LettersSet es vacío
     * @return  true si el LettersSet está vacío \n
     *          false en caso contrario
     */
    bool empty () const;

    /**
     * @brief Tamaño del LettersSet
     * @return Número de elementos en el LettersSet
     */
    unsigned int size () const;

    /**
     * @brief Calula la puntuación dada una palabra
     * @param word String con la palabra cuya puntuación queremos calcular
     * @return Puntuación de la palabra, calculada como la suma de las puntuaciones de cada una de sus letras
     * @post Si la palabra posee una letra no añadida al conjunto, puntuará como 0
     */
    int getScore (string word);

    /**
     * @brief Sorbecarga del operador de asignación
     * @param cl LettersSet a copiar
     * @return Referencia al objeto this para poder encadenar el operador
     */
    LettersSet & operator= (const LettersSet & cl);

    /**
     * @brief Sobrecarga del operador de consulta
     *
     * Permite acceder a los elementos del map que hay en nuestra clase
     * @param val Carácter a consultar
     * @return  Estructura de tipo LetterInfo con la información del carácter consultado:
     *          Número de repeticiones y puntuación
     */
    LetterInfo & operator[] (const char & val);

    /**
     * @brief Sobrecarga constante del operador de consulta
     *
     * Permite acceder a los elementos del map que hay en nuestra clase
     * @param val Carácter a consultar
     * @return  Estructura de tipo LetterInfo con la información del carácter consultado:
     *          Número de repeticiones y puntuación
     */
    const LetterInfo & operator[] (const char & val) const;

    /**
     * @brief Sobrecarga del operador de salida
     * @param os Flujo de salida, donde escribir el LettersSet
     * @param cl LettersSet que se escribe
     */
    friend ostream & operator<< (ostream & os, const LettersSet & cl);

    /**
     * @brief Sobrecarga del operador de entrada
     * @param is Flujo de entrada, del que leer el LettersSet
     * @param cl LettersSet en el que almacenar la información leída
     * @return
     */
    friend istream & operator>> (istream & is, LettersSet & cl);

    /**
     * @brief Clase iteradora de la clase Letters_set
     *
     * Nos permite iterar sobre los elementos de un Letters_set
     */
    class iterator{
    private:
        map<char, LetterInfo>::const_iterator it;

    public:
        iterator() = default;

        iterator(const map<char, LetterInfo>::const_iterator & i){

            it=i;

        }

        const LettersSet::iterator & operator=(const LettersSet::iterator &other){

            it=other.it;
            return(*this);

        }

        const LettersSet::iterator & operator=(const map<char, LetterInfo>::iterator &other){

            it=other;
            return(*this);

        }

        const pair<char, LetterInfo> operator*() const{
            return(*it);
        }

        LettersSet::iterator & operator++() {

            this->it++;
            return(*this);

        }

        LettersSet::iterator & operator--() {

            this->it--;
            return(*this);

        }

        bool operator==(const map<char, LetterInfo>::const_iterator & i ) {

            return(it==i);

        }

        bool operator== (const LettersSet::iterator & other){
            return (it == other.it);
        }

        bool operator!=(const map<char, LetterInfo>::const_iterator & i) {

            return(it!=i);

        }

        bool operator!= (const LettersSet::iterator & other){
            return (it != other.it);
        }


    };


    LettersSet::iterator begin() const;

    LettersSet::iterator end() const;

};

#endif // __LETTER_SET_H__
