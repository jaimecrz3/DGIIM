/**
 * @file maxqueue.h
 * @brief  Archivo de especificación del TDA MaxQueue
 * @author
 */


#ifndef _MAXQUEUE_H_
#define _MAXQUEUE_H_

#include <iostream>
#include <cstdlib>
#include <stack>

using namespace std;

/**
 * @struct element
 * @brief Contenedor usado para almacenar un valor y el máximo valor registrado en la estructura
 */
struct element{
    int value; // Current value to store
    int maximum; //Current max value in the structure
};


/**
  @class MaxQueue
  @brief T.D.A. MaxQueue

  Una instancia del contenedor MaxQueue nos permite usar una Cola (Queue) en la que se tendrá registro en todo momento
  del mayor elemento almacenado en la cola

  Para poder usar el TDA MaxQueue se debe incluir el fichero

  \#include <maxqueue.h>

  @author Jaime Corzo
  @author Mario Líndez
  @date Octubre 2022

**/

class MaxQueue {
private :

    /**
      @brief Pila contenedora
      Estructura usada para representar el MaxQueue "por debajo"
    **/

    stack<element> pila;


    /**
      @brief Copy un MaxQueue .
      @param p Referencia al MaxQueue original que vamos a copiar
      @pre Asume this != &orig
    **/
    void copy(const MaxQueue & p);


public :
    /**
     * @brief Constructor primitivo
     * @return Crea una MaxQueue vacía
     */
    MaxQueue();

    /**
      * @brief Constructor de copias.
      * @param c: MaxQueue que se copia.
      * @return Crea una MaxQueue que es copia de @p c
      */
    MaxQueue (const MaxQueue & p);

    /**
      * @brief Función para conocer si una cola está vacía.
      * @return true: Si la cola está vacía \n
      *         false: En otro caso
      */
    bool empty() const;

    /**
      * @brief Borra todos los elementos.
      * @post Deja el contenedor completamente vacio.
      */
    void clear();

    /**
     * @brief Acceso al elemento al principio de la cola.
     * @pre El receptor no puede estar vacio: !empty
     * @return Referencia al elemento en el frente de la cola
    */
    element & front();

    /**
     * @brief Acceso al elemento al principio de la cola.
     * @pre El receptor no puede estar vacio: !empty
     * @return Referencia constante al elemento en el frente de la cola
    */
    const element & front() const;

    /**
     * @brief Añade un elemento en la cola
     * @param elem: Elemento que se inserta
     * @post Inserta un nuevo elemento al end de la cola.
     * @post Los elementos anteriores de la cola verán modificado su máximo registrado. Esto se debe a que el nuevo
     *       elemento `X` insertado será el último en salir de la cola, por lo que si éste es el valor máximo se deberá
     *       ver reflejado en los elementos por encima de él.
    */
    void push(const int & elem);

    /**
     * @brief Quita un elemento de la cola.
     * @pre El receptor no puede estar vacio: !empty
     * @post Elimina el elemento en el frente de la cola
    */
    void pop();

    /**
      * @brief Devuelve el número de elementos.
      * @return número de elementos de la cola.
      */
    int size() const;

    /**
     * @brief Intercambia el contenido del receptor y del argumento
     * @param p Cola a intercambiar con el receptor
     * @post Este método asigna el contenido del receptor al del parámetro y el del parámetro al receptor
     */
    void swap (MaxQueue & p);
} ;


#endif // _MAXQUEUE_H_

/**
 * @brief Muestra por pantalla un dato de tipo elemento
 * @param flujo
 * @param elem
 * @return
 */
ostream & operator << (ostream & flujo, const element & elem);