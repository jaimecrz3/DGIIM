//
// Created by marta26 on 10/04/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <set>
#include <random>
#include <cstdlib>
#include <chrono>


const int UMBRAL = 1;
using namespace std;
using namespace std::chrono;

struct Punto{
    double x;
    double y;
    int pos_alcanzables;

    // Constructor con parámetros
    Punto(double i, double j){
        x=i;
        y=j;
        pos_alcanzables=0;
    }
};

// Functor para ordenar el vector de puntos respecto de la coordenada x
bool ordenar_x(Punto p1, Punto p2) {
    bool devolver = p1.x < p2.x;
    if (p1.x == p2.x){
        devolver = p1.y < p2.y;
    }

    return (devolver);
}
bool ordenar_y(Punto p1, Punto p2){

    bool devolver = p1.y<p2.y;
    if(p1.y==p2.y){
        devolver=p1.x<p2.x;
    }
    return devolver;
}

//Función que determina si un punto p1 alcanza a otro punto p2
bool punto_alcanza(Punto p1, Punto p2){  // p1 alcanza a p2

    bool alcanza = false;
    if((p1.x > p2.x) && (p1.y > p2.y)) alcanza = true;

    return alcanza;
}

// Función que contiene el algoritmo básico
vector<Punto> dyv_basico (vector<Punto> &a) {

    //Recorremos todos los puntos y vamos comprobando si se alcanzan
    for (int x = 0; x < a.size(); x++) {
        for (int y = 0; y < a.size(); y++) {
            if (x != y && punto_alcanza(a[x], a[y])){
                a[x].pos_alcanzables++;
            }
        }
    }

    vector<Punto> sol=a;
    return sol;
}

// Función que fusiona dos vectores y añade las posiciones alcanzables de cada elemento
vector<Punto> Fusion (const vector<Punto> &izq, const vector<Punto> &dcha){

    //Creo un vector donde guardar la solución
    vector<Punto> solucion;  //Los puntos se van a ir almacenando en el vector en orden creciente de la coordenada y

    int tam_izq = izq.size();
    int tam_dcha = dcha.size();

    //El max de las coordenadas x del vector de la izq
    double max_izq = max_element(izq.begin(),izq.end(), ordenar_x)->x;


    int i = 0; //posicion vector izquierda
    int d = 0; //posicion vector derecha
    int alcanzables = 0; //Variable donde vamos almacenando el numero de puntos que son alcanzables(por los de la dcha)
    int alcanzables_estricto= 0;   //Igual que alcanzables pero sin contar los puntos en los que la coordenada x
// del punto de la izq es la misma que max_izq, por lo tanto si un punto de la der tiene tambien la misma coordenada x
// (una menor no podria ser porque entonces estaria en el vector de la izq, recordemos que max_izq es el max)
// implica que no alcanza a los que tienen su misma coordenada x(tiene que ser mayor estricto para ser alcanzable)

    // Tomamos un punto de cada vector
    Punto pto_dcha = dcha.at(d);
    Punto pto_izq = izq.at(i);

    do{
        // Tomamos un punto de cada vector
        if (d < tam_dcha) {
            pto_dcha = dcha.at(d);
        }

        if (i < tam_izq) {
            pto_izq = izq.at(i);
        }

        bool en_lim = (d == tam_dcha || i == tam_izq);

        //Primero comprobamos si d o i han llegado al tope de su vector, si ha sido d, simplemente añadimos
        //los puntos de la izq al vector de solucion ya que estos no van a alcanzar a los de la derecha
        if (en_lim) {
            if (d == tam_dcha) {
                while (i < tam_izq) {
                    solucion.push_back(izq.at(i));
                    i++;
                }
            }
            else {
                if (i == tam_izq) {         //Si ha sido la i, al añadir los puntos de la derecha si tenemos que
                    //añadir sus puntos alcanzables
                    while (d < tam_dcha) {
                        solucion.push_back(dcha.at(d));
                        if (dcha.at(d).x == max_izq) {
                            solucion.back().pos_alcanzables += alcanzables_estricto;
                        } else {
                            solucion.back().pos_alcanzables += alcanzables;
                        }
                        d++;
                    }
                }
            }
        }
        else {
            // Solo falta comprobar la coordenada y porque la x siempre será mayor o igual en derecha
            if ((pto_izq.y < pto_dcha.y)) {
                //Este es el caso en que pto_dcha puede alcanzar a pto_izq si la la x tambien es mayor estricto
                solucion.push_back(pto_izq);
                alcanzables++;
                if (pto_izq.x != max_izq) {
                    alcanzables_estricto++;
                }
                i++;
            }
            else{
                solucion.push_back(pto_dcha);   //Si es mayor o igual el de la izq, el de la derecha no va a alcanzar
                // ni a este ni a los siguientes de la izq, pero
                //hay que añadir los puntos anteriores a los que si alcanza
                if (pto_dcha.x == max_izq) {
                    solucion.back().pos_alcanzables += alcanzables_estricto;
                } else {
                    solucion.back().pos_alcanzables += alcanzables;
                }

                d++;
            }
        }
    }while(d<tam_dcha||i<tam_izq);

    return solucion;

}


vector<Punto> DivideYVenceras (const vector<Punto> &v,int inf, int sup ){
    vector<Punto> solucion;

    int tam_v=sup-inf;

    // condicion parada: el vector tiene un tamaño menor o igual que el umbral
    if (tam_v > UMBRAL) {
        // llamada recursiva con los dos nuevos vectores
        vector<Punto> izq=DivideYVenceras(v, inf, ceil((double)(inf+sup)/2));
        vector<Punto> dcha=DivideYVenceras(v,ceil((double) (inf+sup)/2) , sup);

        // unir los vectores
        solucion = Fusion(izq,dcha);

    }
    else{
        vector<Punto> aux;
        for(int i = inf; i< sup; i++) aux.push_back(v.at(i));
        solucion = dyv_basico(aux);
        sort(solucion.begin(), solucion.end(), ordenar_y);
    }

    return solucion;
}

vector<Punto> DivideYVenceras (const vector<Punto> &v){

    return (DivideYVenceras(v,0,v.size()));
}

bool son_iguales(Punto P1, Punto P2){
    return (P1.x != P2.x || P1.y != P2.y);
}

int main(int argc, char* argv[]) {


    if (argc != 2){
        cerr << "Formato " << argv[0] << " <fichero_puntos>" << endl;
        return -1;
    }

    char *nom_fich= argv[1];

    ifstream fin;
    fin.open(nom_fich);
    int num_puntos=-1;
    vector<Punto> puntos;

    if(fin){

        while(fin){
            double i,j;
            fin >> i;
            fin >> j;
            if(fin){
                Punto p(i,j);
                cout << "Punto (" << p.x <<  "," << p.y << ")" << endl;
                puntos.push_back(p);
            }
            num_puntos++;
        }
    }
    else{
        cout << "ERROR: no se ha podido abrir el fichero que contiene los puntos." << endl;
    }

    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;
    t_antes = high_resolution_clock::now();
    sort(puntos.begin(),puntos.end(), ordenar_x);
    vector <Punto> sol;
    sol=DivideYVenceras(puntos);

    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << num_puntos << "  " << transcurrido.count() << endl;
    /*
    sort(sol.begin(),sol.end(), ordenar_x);
    for(int i=0; i<sol.size();i++){
        cout << "Punto (" << sol[i].x <<  "," << sol[i].y << "):"  << sol[i].pos_alcanzables << endl;
    }

    cout << endl;

    dyv_basico(puntos);
    sort(puntos.begin(),puntos.end(), ordenar_x);
    for(int i=0; i<puntos.size();i++){
        cout << "Punto (" << puntos[i].x <<  "," << puntos[i].y << "):"  << puntos[i].pos_alcanzables << endl;
    }
     */

    return 0;
}
