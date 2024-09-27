#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <set>
#include <random>

using namespace std;
using namespace std::chrono;


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
bool alcanza(Punto p1,Punto p2){

    bool alcanza=false;

    if((p1.x>p2.x) && (p1.y>p2.y)){
        alcanza=true;
    }

    return(alcanza);
}

void especifico(vector<Punto> &a){

    //Recorremos todos los puntos y vamos comprobando si se alcanzan
    for(int i= 0; i<a.size(); i++){
        for(int j=0; j<a.size(); j++){
            if(i!=j && alcanza(a[i],a[j])){
                a[i].pos_alcanzables++;
            }
        }
    }

}


int main(int argc, char *argv[]) {

    if (argc != 2)
    {
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
    const int MIN=0, MAX=5000;
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
        //cout << "Punto (" << p.x <<  "," << p.y << "):"  << endl;
        posiciones.insert(p);
        ++k;

    }

    vector<Punto> puntos(posiciones.begin(), posiciones.end());
    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    especifico(puntos);

    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << n << "  " << transcurrido.count() << endl;
	
	/*
    vector<Punto>::iterator it;
    for(it=puntos.begin(); it!=puntos.end(); it++){
        int num =(*it).pos_alcanzables;
        cout << "Punto (" << (*it).x <<  "," << (*it).y << "): " << num << endl;
    }
    */

    return 0;
}
