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
// ENTRADA: numero de clientes(n) y numero de camareros(c)

// Conjunto de candidatos. lista con prioridad o vector o lo que sea(el profesor dijo que no nos complicasemos,
// un vector

// Cada cliente tiene una variable que es al distancia

struct Cliente{
    double distancia;
    double tiempo_espera=0;  //Va a ser (el que le atienda), camarero.distancia_actual + cliente.distancia
};

struct Camarero{
    double distancia_actual=0;
};

bool orden_dist_cliente (Cliente c1, Cliente c2){
    return(c1.distancia<c2.distancia);
}

/**
 *
 * @param camareros
 * @param clientes
 * @pre clientes tiene que estar ordenado de menor a mayor distancia
 * @return tiempo_espera_medio
 */
double Voraz(vector<Camarero> &camareros, vector<Cliente> &clientes){

    double tiempo_espera_medio;
    double suma_tiempos_espera=0;

    int i=0;
    int j=0;
    while(i < clientes.size()) {

        //Aumentamos el tiempo de espera
        clientes[i].tiempo_espera += camareros[j].distancia_actual + clientes[i].distancia;
        //Actualizamos la suma de todos los tiempos de espera
        suma_tiempos_espera += clientes[i].tiempo_espera;


        //Actualizamos la distancia del camarero j-ésimo
        camareros[j].distancia_actual += 2 * (clientes[i].distancia); //La ida y la vuelta
        //cout << cliente_seleccionado.tiempo_espera << endl;

        i++;

        j++;
        if (j == camareros.size()) {
            j = 0;
        }

    }

    //Calculamos el tiempo de espera medio
    tiempo_espera_medio = suma_tiempos_espera/clientes.size();

    return(tiempo_espera_medio);


}



int main(int argc, char *argv[]){

    if (argc != 2){
        cerr << "Formato " << argv[0] << " <fichero_clientes>" << endl;
        return -1;
    }

    char *nom_fich= argv[1]; // Nombre del fichero donde están los datos de entrada

    ifstream fin;
    fin.open(nom_fich);
    int num_clientes=0;
    vector<Cliente> candidatos;
    vector<Camarero> camareros;
    int num_camareros;

    // Lectura de los datos y se guardan en un  vector
    if(fin){

        fin >> num_camareros;
        if(!fin){
            cout << "ERROR: no se ha podido leer el fichero que contiene los clientes." << endl;
        } else{
            for (int i = 0; i < num_camareros; i++) {
                Camarero nuevo;
                camareros.push_back(nuevo);
            }
        }

        while(fin){
            double d;
            fin >> d;
            if(fin){
                Cliente cliente;
                cliente.distancia = d;
                candidatos.push_back(cliente);
                num_clientes++;
            }

        }
    }
    else{
        cout << "ERROR: no se ha podido abrir el fichero que contiene los clientes." << endl;
    }

    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();

    // Ordenamos los clientes de menor a mayor por la distancia a la que están
    sort(candidatos.begin(), candidatos.end(),orden_dist_cliente);
    double tiempo_espera_medio= Voraz(camareros, candidatos);

    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << "num clientes: " << num_clientes << " num camareros: " << num_camareros << "  " << transcurrido.count() << endl;


    cout << "El tiempo de espera medio por cliente es: " << tiempo_espera_medio << endl;
    cout << "CLIENTES" << endl;

    for(int i = 0; i < candidatos.size(); i++){

        cout << "Cliente: " << i << " Distancia: " << candidatos.at(i).distancia << " Tiempo: " << candidatos.at(i).tiempo_espera << endl;
    }

    return 0;
}



