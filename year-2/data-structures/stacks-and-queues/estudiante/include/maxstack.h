/**
 * @file maxstack.h
 * @brief  Archivo de especificación del TDA MaxStack
 * @author
 */

#ifndef _MAXSTACK_H_
#define _MAXSTACK_H_

#include <iostream>
#include <cstdlib>
#include <queue>

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
  @class MaxStack
  @brief T.D.A. MaxStack

  Una instancia del contenedor MaxStack nos permite usar una Pila (Stack) en la que se tendrá registro en todo momento
  del mayor elemento almacenado en la pila

  Para poder usar el TDA MaxStack se debe incluir el fichero

  \#include <maxstack.h>

  @author Jaime Corzo
  @author Mario Líndez
  @date Octubre 2022

**/

class MaxStack{
private :

    /**
      @brief Cola contenedora
      Estructura usada para representar el MaxStack "por debajo"
    **/

    queue<element> cola;


    /**
      @brief Copy un MaxStack .
      @param p Referencia al MaxStack original que vamos a copiar
      @pre Asume this != &orig
    **/
    void copy(const MaxStack & p);


public :
    /**
     * @brief Constructor por defecto
     * @return Un objeto MaxStack vacío
     */
    MaxStack();

    /**
      * @brief Constructor de copias.
      * @param p: pila que se copia.
      * @return Pila, el objeto MaxStack creado.
      */
    MaxStack (const MaxStack & p);

    /**
      * @brief Funcion para conocer si una pila está vacía.
      * @return Si la pila está vacía
      * @post la pila no se modifica.
      */
    bool empty() const;

    /**
      * @brief Devuelve el número de elementos.
      * @return número de elementos de la pila.
      */
    int size() const;

    /**
      * @brief Borra todos los elementos.
      * @post Deja la pila completamente vacia.
      */
    void clear();

    /**
     * @brief Acceso al elemento en lo alto de la pila.
     * @pre El receptor no puede estar vacio: !empty
     * @return Referencia al elemento en el tope de la pila
    */
    element & top();

    /**
     * @brief Acceso al elemento en lo alto de la pila.
     * @pre El receptor no puede estar vacio: !empty
     * @return Referencia constante al elemento en el tope de la pila
    */
    const element & top() const;

    /**
     * @brief Deposita un elemento en la pila
     * @param elem: Elemento que se inserta
     * @post Inserta un nuevo elemento en la pila. Dicho elemento se situa en el tope de la pila
     * @post Los elementos ya existentes en la pila no se verán modificados. Esto se debe a que si un elemento `X` de la
     *       pila se le añade un elemento `Y` mayor por encima de él, cuando se pueda volver a acceder a ´X´,
     *       ese `Y` ya no se encontrará en la pila.
    */
    void push(const int & elem);

    /**
     * @brief Quita un elemento de la pila.
     * @pre El receptor no puede estar vacio: !empty
     * @post Elimina el elemento en el tope de la pila
    */
    void pop();


    /**
     * @brief Intercambia el contenido del receptor y del argumento
     * @param p Pila a intercambiar con el receptor
     * @post Este método asigna el contenido del receptor al del parámetro y el del parámetro al receptor
     */
    void swap (MaxStack & p);

} ;


#endif // _MAXSTACK_H_

/**
 * @brief Muestra por pantalla un dato de tipo elemento
 * @param flujo
 * @param elem
 * @return
 */
ostream & operator << (ostream & flujo, const element & elem);