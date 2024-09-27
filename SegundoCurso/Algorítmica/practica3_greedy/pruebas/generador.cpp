//
// Created by sarita on 20/04/23.
//
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// El generador de casos va a ser genérico de manera que se van crear tan solo las distancias entr cada uno de los nodos
// Sera un amtriz simetrica con 0 en la diagonal
// Como parametro de entrada tendremos el numero de nodos

int main(int argc, char* argv[]) {

    if (argc != 2){
        cerr << "Formato " << argv[0] << " <num_ciudades>" << endl;
        return -1;
    }

    int n = atoi(argv[1]);

    if (n < 0){
        cerr << "El numero de ciudades ha de ser positivo" << endl;
        return -1;
    }

    ofstream fo;
    fo.open("ficheroDeSalida.txt");

    // Mnadar el numero de ciudades

    fo << n << endl;
    //Generamos los puntos aleatoriamente
    srandom(time(0));
    const int MIN = 1, MAX = 1000;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(MIN, MAX);

    double distancia;

    vector<vector<double>> matriz;
    vector<double> aux;

    for(int i=0; i<n; i++){
        aux.push_back(0.0);
    }
    for(int i=0; i<n; i++){
        matriz.push_back(aux);
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i<j){
                distancia = distr(eng);
                matriz.at(i).at(j) = distancia;
                matriz.at(j).at(i) = distancia;
            }

            fo << setw(10 ) << matriz.at(i).at(j) << " ";
        }

        fo << endl;
    }

    fo << endl;
    fo.close();

    return 0;
}
