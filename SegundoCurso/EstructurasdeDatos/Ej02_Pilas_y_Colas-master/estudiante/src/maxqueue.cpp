/**
 * @file maxqueue.cpp
 * @brief  Archivo de implementación del TDA MaxQueue
 * @author
 */

#include <iostream>

#include <maxqueue.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/

void MaxQueue::copy(const MaxQueue &p) {

    stack<element>aux(p.pila);

    pila = aux;

}

/********************************
      FUNCIONES PUBLICAS
********************************/
MaxQueue::MaxQueue() = default;


MaxQueue::MaxQueue(const MaxQueue & p) {

    copy(p);

}

bool MaxQueue::empty() const {
    return (pila.empty());

}

int MaxQueue::size() const {
    return (pila.size());

}

void MaxQueue::clear() {

    int tamanio = size();

    for (int i = 0; i < tamanio; ++i){
        pila.pop();
    }

}

element & MaxQueue::front() {
    return (pila.top());
}

const element & MaxQueue::front() const {

    return (pila.top());

}

void MaxQueue::push(const int & elem){

    element nuevo;
    nuevo.value = elem;

    if(empty()){

        nuevo.maximum = nuevo.value;
        pila.push(nuevo);

    } else {

        stack<element> aux;
        int n = pila.size();

        for (int i = 0; i < n; ++i){
            aux.push(pila.top());
            pila.pop();
        }

        nuevo.maximum = nuevo.value;
        pila.push(nuevo);

        for (int i = 0; i < n; ++i){

            if(nuevo.maximum >= aux.top().maximum){

               aux.top().maximum = nuevo.maximum;

            }

            pila.push(aux.top());
            aux.pop();

        }

    }


}

void MaxQueue::pop() {

    pila.pop();

}

void MaxQueue::swap(MaxQueue &p) {
    pila.swap(p.pila);

}

//Sobrecarga del operador <<
ostream & operator << (ostream & flujo, const element & elem){
    flujo << elem.value << "," << elem.maximum;

    return flujo;
}