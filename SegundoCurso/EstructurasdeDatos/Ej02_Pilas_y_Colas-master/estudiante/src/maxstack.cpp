/**
 * @file maxstack.cpp
 * @brief  Archivo de implementación del TDA MaxStack
 */

#include <iostream>

#include <maxstack.h>

using namespace std;

/********************************
      FUNCIONES PRIVADAS
********************************/

void MaxStack::copy(const MaxStack &p) {

    queue<element>aux(p.cola);

    cola = aux;

}

/********************************
      FUNCIONES PUBLICAS
********************************/
MaxStack::MaxStack() = default;


MaxStack::MaxStack(const MaxStack & p) {

    copy(p);

}

bool MaxStack::empty() const {
    return (cola.empty());

}

int MaxStack::size() const {
    return (cola.size());

}

void MaxStack::clear() {

    int tamanio = size();

    for (int i = 0; i < tamanio; ++i){
        cola.pop();
    }

}

element & MaxStack::top() {

    return (cola.front());
}

const element & MaxStack::top() const {

    return (cola.front());

}

void MaxStack::push(const int & elem){

    element nuevo;
    nuevo.value = elem;

    if(empty()){

        nuevo.maximum = nuevo.value;
        cola.push(nuevo);

    } else {

        queue<element> aux;
        int n = cola.size();

        cola.swap(aux);

        //Calculamos si el elemento nuevo es un máximo
        if(aux.front().maximum >= elem) {

            nuevo.maximum = aux.front().maximum;

        } else{

            nuevo.maximum = nuevo.value;

        }

        cola.push(nuevo);

        //Volcamos el contenido de nuevo a la cola
        for (int i = 0; i < n; ++i){
            cola.push(aux.front());
            aux.pop();
        }

    }


}

void MaxStack::pop() {

    cola.pop();

}

void MaxStack::swap(MaxStack & otro) {
    cola.swap(otro.cola);

}

//Sobrecarga del operador <<
ostream & operator << (ostream & flujo, const element & elem){
    flujo << elem.value << "," << elem.maximum;

    return flujo;
}