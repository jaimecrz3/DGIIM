#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <math.h>
#include<set>
#include <random>

using namespace std;
using namespace std::chrono;



const int UMBRAL = 3;

struct Punto{
    double x;
    double y;
    int pos_alcanzables;

    Punto(double i, double j){
        x=i;
        y=j;
        pos_alcanzables=0;
    }

    bool operator==(Punto P1){
        return (P1.x == this->x && P1.y == this->y);
    }

    bool operator<(const Punto& p2) const{
        bool menor = false;

        if(this->x < p2.x && this->y < p2.y){
            menor=true;
        }

        return menor;
    }

};


bool son_iguales(Punto P1, Punto P2){
    return (P1.x != P2.x || P1.y != P2.y);
}

bool ordenar_x(Punto p1, Punto p2){

    if(p1.x==p2.x) return (p1.y < p2.y);
    else return (p1.x < p2.x);
}


bool punto_alcanza(Punto p1, Punto p2){  // p1 alcanza a p2

    bool alcanza = false;
    if((p1.x > p2.x) && (p1.y > p2.y)) alcanza = true;

    return alcanza;
}

void dyv_basico (vector<Punto> &a) {

    //Recorremos todos los puntos y vamos comprobando si se alcanzan
    for (int x = 0; x < a.size(); x++) {
        for (int y = 0; y < a.size(); y++) {
            if (x != y && punto_alcanza(a[x], a[y])){
                a[x].pos_alcanzables++;
            }
        }
    }
}


vector<Punto> Fusion (const vector<Punto> &izq, const vector<Punto> &dcha){
    //Creo un pair donde guardar la solución
    vector<Punto> solucion;

    int tam_izq = izq.size();
    int tam_dcha = dcha.size();

    double max_izq = izq.back().x;


    int i = 0; //posicion vector izquierda
    int d = 0; //posicion vector derecha
    int alcanzables = 0;
    int alcanzables_estricto = 0;

    Punto pto_dcha = dcha.at(d);
    Punto pto_izq = izq.at(i);

    do{
        // Tomamos un punto de cada vector (van a tener los mínimos valores de abscisas)
        if (d < tam_dcha) {
            pto_dcha = dcha.at(d);
        }

        if (i < tam_izq) {
            pto_izq = izq.at(i);
        }

        bool en_lim = (d == tam_dcha || i == tam_izq);

        if (en_lim) {
            if (d == tam_dcha) {
                while (i < tam_izq) {
                    solucion.push_back(izq.at(i));
                    alcanzables++;

                    if (izq.at(i).x != max_izq) {
                        alcanzables_estricto++;
                    }
                    i++;
                }
            }
            else {
                if (i == tam_izq) {
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
                solucion.push_back(pto_izq);
                alcanzables++;
                if (pto_izq.x != max_izq) {
                    alcanzables_estricto++;
                }
                i++;
            }
            if (pto_izq.y >= pto_dcha.y) {
                solucion.push_back(pto_dcha);
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

    // condicion parada: el vector tiene un unico elemento
    if (tam_v > 1) {
        // llamada recursiva con los dos nuevos vectores
        vector<Punto> izq=DivideYVenceras(v, inf, ceil((double)(inf+sup)/2));
        vector<Punto> dcha=DivideYVenceras(v,ceil((double) (inf+sup)/2) , sup);

        // unir los vectores
        solucion = Fusion(izq,dcha);

    }
    else if(tam_v==1){  // el vector de puntos tiene solo un elemento
        solucion.push_back(v.at(inf));
    }

    return solucion;
}

vector<Punto> DivideYVenceras (const vector<Punto> &v){

    return (DivideYVenceras(v,0,v.size()));
}

int main(int argc, char* argv[]) {


    if (argc != 2){
        cerr << "Formato " << argv[0] << " <num_puntos>" << endl;
        return -1;
    }

    int n = atoi(argv[1]);

    if (n <= 0){
        cerr << "El numero de puntos ha de ser positivo" << endl;
        return -1;
    }


    //Generamos los puntos aleatoriamente
    bool (*fn_pt)(Punto,Punto)=son_iguales;
    set<Punto,bool (*)(Punto,Punto)> posiciones (fn_pt);

    srandom(time(0));
    const int MIN=0, MAX=500;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(MIN,MAX);

    double i=0;
    double j=0;
    int k=0;
    while(posiciones.size()<n){
        i=distr(eng);
        j=distr(eng);
        Punto p(i,j);
        cout << "Punto (" << p.x <<  "," << p.y << "):"  << endl;
        posiciones.insert(p);
        ++k;

    }
    vector<Punto> puntos(posiciones.begin(), posiciones.end());

    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    sort(puntos.begin(),puntos.end(), ordenar_x);
    vector <Punto> sol;
    sol=DivideYVenceras(puntos);

    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << n << "  " << transcurrido.count() << endl;

    for(int i=0; i<sol.size();i++){
        cout << "Punto (" << sol[i].x <<  "," << sol[i].y << "):"  << sol[i].pos_alcanzables << endl;
    }

    cout << endl;

    dyv_basico(puntos);
    for(int i=0; i<puntos.size();i++){
        cout << "Punto (" << puntos[i].x <<  "," << puntos[i].y << "):"  << puntos[i].pos_alcanzables << endl;
    }


    return 0;
}