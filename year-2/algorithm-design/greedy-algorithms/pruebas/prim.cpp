//
// Created by marta26 on 17/04/23.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>



using namespace std;
using namespace std::chrono;

vector<double> Prim ( const vector<vector<double>> &matriz){

    vector<double> aristas;
    vector<bool> vertices_seleccionados;
    int num_vertices = matriz.size();

    for(int i = 0; i< num_vertices; i++) vertices_seleccionados.push_back(false);

    vertices_seleccionados.at(0) = true;
    int pos_actual = 0;

    while(aristas.size()<num_vertices){

        // Busco la arista mínima conectada a ese vértice
        double min = 9999999;
        int pos_min = pos_actual;

        for (int i = 0; i < num_vertices; ++i) {
            double d = matriz.at(pos_actual).at(i);
            if(d<min && d>0 && !vertices_seleccionados.at(i)){
                min = d;
                pos_min = i;
            }
        }

        vertices_seleccionados.at(pos_min) = true;
        aristas.push_back(matriz.at(pos_actual).at(pos_min));
        pos_actual = pos_min;
    }

    aristas.back()= matriz.at(pos_actual).at(0);
    /*
    vector<double> aristas;
    vector<int> mas_prox; // aristas candidatas a ser elegidas
    vector<double> dist_min;

    int num_vertices = matriz.size();

    // Introducimos en mas_prox
    for(int i=0; i<num_vertices; i++){
        mas_prox.push_back(0);
        dist_min.push_back(matriz.at(i).at(0));
    }

    int pos_min=0;

    for(int i=0; i<num_vertices-1;i++){
        double min = 999999;
        // Busco la arista mínima conectada a ese vértice
        for(int j=1; j<num_vertices; j++){
            double d = dist_min.at(j);
            if(d<min && d>0){
                min = dist_min.at(j);
                pos_min = j;
            }
        }

        // Añado a las soluciones
        aristas.push_back(dist_min.at(pos_min));

        dist_min.at(pos_min) = -1;

        for(int j=1; j<num_vertices; j++){
            double arista = matriz.at(j).at(pos_min);
            double dist = dist_min.at(j);

            if(arista<dist){
                dist_min.at(j) = arista;
                mas_prox.at(j) = pos_min;
            }
        }
    }

    aristas.push_back(matriz.at(0).at(pos_min));
     */


    return aristas;

}


int main(int argc, char* argv[]){

    if (argc != 2 ){
        cerr << "Formato " << argv[0] << " <fichero_distancias>" << endl;
        return -1;
    }

    char *fich = argv[1];

    ifstream fin;
    fin.open(fich);

    int num_ciudades;
    vector<vector<double>> matriz_dist;

    if(fin){
        fin >> num_ciudades;

        vector<double> aux;
        for(int i = 0; i < num_ciudades; i++){
            for(int j = 0; j < num_ciudades;j++){
                double distancia;
                fin >> distancia;

                if(fin) {
                    aux.push_back(distancia);
                }
            }
            matriz_dist.push_back(aux);
            aux.clear();
        }

        /*
        for(int i=0; i< matriz_dist.size(); i++){

            cout << i << ": ";
            for(int j=0; j<matriz_dist.size(); j++){
                cout << setw(10) << matriz_dist.at(i).at(j )<< " ";
            }
            cout << endl;
        }

         */
        fin.close();
    }
    else{
        cout << "ERROR: no se ha podido abrir el fichero que contiene los puntos." << endl;
        return -1;
    }


    vector<double> sol;
    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    sol = Prim(matriz_dist);
    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << num_ciudades << "  " << transcurrido.count() << endl;

    /*
    cout << endl << "ARISTAS" << endl;


    for(int k=0; k<sol.size();k++) {
        cout << sol.at(k) << endl;
    }
     */

    return 0;

}
