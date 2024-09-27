#ifndef __BAG_H__
#define __BAG_H__

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 *  @brief TDA abstracto Bolsa
 *
 *  Este TDA abstracto nos permite trabajar con una colección de elementos que
 *  permite la extracción de elementos de forma aleatoria sin reemplazamiento
 */
template <class T>
class Bag {
private:
    vector<T> v;

public:
    /**
     * @brief Constructor por defecto
     */
    Bag() = default;

    /**
     * @brief Constructor de copia
     *
     * Crea una copia exacta de otro objeto de tipo Bag
     * @param other Objeto de tipo Bag<T> del que se va a realizar la copiar
     */
    Bag (const Bag<T> & other){
        v = other.v;
    }

    /**
     * @brief Añade un elemento a la bolsa
     * @tparam element Elemento del tipo T a añadir a la bolsa
     */
    void add(const T & element){
        v.push_back(element);
    }

    /**
     * @brief Extrae un elemento aleatorio de la bolsa
     *
     * Devuelve un elemento aleatorio de la bolsa y lo elimina de la misma
     * @return Elemento de tipo T extraído de la bolsa
     * @pre La bolsa no está vacía
     * @post El elemento devuelto se ha eliminado de la bolsa
     */
    T get(){

        //Genera un numero aleatorio entre 0 y size-1
        int pos = rand()%(v.size());

        T element = v[pos];

        v[pos] = v[v.size() - 1];
        v.pop_back();

        return (element);
    }

    /**
     * @brief Elimina todos los elementos de la bolsa
     *
     * Borra todos los elementos almacenados en la bolsa
     */
    void clear(){
        v.clear();
    }

    /**
     * @brief Tamaño de la bolsa
     * @return Número de elementos que hay en la bolsa
     */
    unsigned int size () const {
        return (v.size());
    }

    /**
     * @brief Comprueba si la bolsa está vacía
     * @return  true si la bolsa está vacía \n
     *          false en case contrario
     */
    bool empty(){
        return (v.empty());
    }

    /**
     * @brief Sobrecarga del operador de asignación
     * @param other Bag<T> a copiar
     * @return Referencia a this para poder encadenar el operador
     */
    const Bag<T> & operator= (const Bag <T> & other){
        if (&other != this){
            v.clear();
            v = other.v;
        }

        return (*this);
    }

};

#endif
