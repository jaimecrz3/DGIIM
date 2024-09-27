#include <iostream>
#include<vector>
#include<algorithm>
#include<list>
#include <cstdlib>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Cliente{
    double distancia;
    double tiempo_espera=0;  //Va a ser (el que le atienda), camarero.distancia_actual + cliente.distancia
};

struct Camarero{
    double distancia_actual=0;
};


int main(int argc, char* argv[]) {


    if (argc != 2){
        cerr << "Formato " << argv[0] << " <num_camareros>" << endl;
        return -1;
    }

    int n = atoi(argv[1]);
    //Se toma el número de camareros constante
    int c=10;

    if (n <= c || n>100){
        cerr << "El numero de camareros ha de ser mayor estricto que los camareros(10) y menor que 100" << endl;
        return -1;
    }


    ofstream fo;
    fo.open("ficheroDeSalida.txt");
    //Generamos los puntos aleatoriamente
    vector<Cliente> clientes;

    srandom(time(0));
    const int MIN=1, MAX=100;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(MIN,MAX);

    //Le pasamos el numero de camareros
    fo << c << endl;

    int k=0;
    while (k < n) {

        Cliente cliente;
        cliente.distancia = distr(eng);

        fo << cliente.distancia << endl;
        k++;

    }

    fo << endl;
    fo.close();

    return 0;
}
