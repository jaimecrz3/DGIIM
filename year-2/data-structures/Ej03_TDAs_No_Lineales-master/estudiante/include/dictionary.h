#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <set>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 * @brief TDA Dictionary
 * @details Almacena las palabras de un fichero de texto y permite iterar sobre ellas
 *
 */

class Dictionary {
private:
    set <string> words;

public:
public:
    /**
     * @brief Constructor por defecto
     */
    Dictionary() = default;

    /**
     * @brief Constructor de copia
     * @details Crea un dictionary con el mismo contenido que el que se pasa como argumento
     * @param other Dictionary que se quiere copiar
     */

    Dictionary (const Dictionary & other);

    /**
     * @brief Indica si una palabra está en el diccionario o no
     * @param val Palabra que se quiere buscar
     * @return booleano indicando si la palabra está o no
     */
    bool exists(const string & val)const;

    /**
     * @brief Inserta una palabra en el diccionario
     * @param val Palabra que se quiere buscar
     * @return booleano indicando si inserccion ha tenido exito, se tiene exito si no existia la palabra en el diccionario
     */
    bool insert(const string & val);

    /**
     * @brief Elimina una palabra del diccionario
     * @param val Palabra a borrar del diccionario
     * @return booleano indicando si la palabra se ha borrado del diccionario
     */
    bool erase(const string & val);


    /**
     * @brief Vacía el Diccionario
     *
     * Elimina todas las palabras contenidas en el conjunto
     */
    void clear ();


    /**
     * @brief Tamaño del Diccionario
     * @return numero de palabras guardadas en el diccionario
     */
    unsigned int size () const;

    /**
     * @brief comprueba si el diccionario está vacio
     * @return true si el diccionario esta vacio
     */

    bool empty() const;

    /**
     * @brief Indica el numero de apariciones de una letra
     * @param c letra a buscar
     * @return Un entero indicando el numero de apariciones
     */

    int getOcurrences(const char c);

    /**
     * brief Cuenta el total de letras en diccionario
     * @return Entero con el total de letras
     */

    int getTotalLetters();

    /**
     * @brief Devuelve las palabras en el diccionario con una longitud dada
     * @param length longitud de las palabras buscadas
     * @return vector de palabras con la longitud deseada
     */

    vector<string> wordsOfLength(int length);

    /**
     * @brief Sobrecarga del operador de salida
     * @param os Flujo de salida, donde escribir el Dictionary
     * @param cl Dictionary que se escribe
     */
    friend ostream & operator<< (ostream & os, const Dictionary & cl);

    /**
     * @brief Sobrecarga del operador de entrada
     * @param is Flujo de entrada, del que leer el Dictionary
     * @param cl Dictionary en el que almacenar la información leída
     * @return
     */
    friend istream & operator>> (istream & is, Dictionary & cl);

    /**
     * @brief Clase iteradora de la clase Dictionary
     *
     * Nos permite iterar sobre los elementos de un dictionary
     */
    class iterator{
    private:
        set<string>::const_iterator it;

    public:
        iterator() = default;

        iterator(const set<string>::const_iterator & i){

            it=i;

        }

        const Dictionary::iterator & operator=(const Dictionary::iterator &other){

            it=other.it;
            return(*this);

        }

        const Dictionary::iterator & operator=(const set<string>::iterator &other){

            it=other;
            return(*this);

        }

        const string & operator*() const{

            return(*it);

        }

        Dictionary::iterator & operator++() {

            it++;
            return(*this);

        }

        Dictionary::iterator & operator--() {

            it--;
            return(*this);

        }

        bool operator==(const set<string>::const_iterator & i ) {

            return(it==i);

        }

        bool operator==(Dictionary::iterator & i ) {

            return(it == i.it);

        }


        bool operator!=(const set<string>::const_iterator & i) {

            return(it!=i);

        }

        bool operator!=(const Dictionary::iterator & i) {

            return(it != i.it);

        }


    };

    /**
     * @brief Permite acceder al primer elemento de la estructura
     * @return Iterdor de la clase Dictionary al primer elemento
     */
    Dictionary::iterator begin() const{
        Dictionary::iterator it = words.begin();
        return (it);
    }

    /**
     * @brief Marca el final de la estructura
     * @return Iterador de la clase Dictionary al elemento siguiente al último
     */
    Dictionary::iterator end() const{
        Dictionary::iterator it = words.end();
        return (it);
    }

};



#endif
