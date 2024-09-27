#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <math.h>
#include <set>
#include <random>
#include <list>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Punto{

    double x;
    double y;
    int pos_alcanzables;
    list<Punto> puntos;

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

    ofstream fo;
    fo.open("ficheroDeSalida.txt");
    //Generamos los puntos aleatoriamente
    bool (*fn_pt)(Punto, Punto) =son_iguales;
    set<Punto, bool (*)(Punto, Punto)> posiciones(fn_pt);

    srandom(time(0));
    const int MIN = 0, MAX = 10;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(MIN, MAX);

    double i = 0;
    double j = 0;
    int k = 0;
    while (posiciones.size() < n) {

        i = distr(eng);
        j = distr(eng);
        Punto p(i, j);
        posiciones.insert(p);
        fo << p.x  << " " << p.y << endl;
        ++k;

    }
    fo << endl;
    fo.close();

    return 0;
}
