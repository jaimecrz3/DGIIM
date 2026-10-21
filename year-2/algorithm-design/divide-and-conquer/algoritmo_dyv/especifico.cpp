#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <set>
#include <random>
#include <fstream>

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
    especifico(puntos);

    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << num_puntos << "  " << transcurrido.count() << endl;

    vector<Punto>::iterator it;
    for(it=puntos.begin(); it!=puntos.end(); it++){
        int num =(*it).pos_alcanzables;
        cout << "Punto (" << (*it).x <<  "," << (*it).y << "): " << num << endl;
    }

    return 0;
}