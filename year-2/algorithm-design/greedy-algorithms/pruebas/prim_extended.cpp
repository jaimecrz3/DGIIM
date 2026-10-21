//
// Created by marta26 on 20/04/23.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;
const double NO_VALID = -1.0;


double Suma_Distancias(const vector<double> &v){

    double suma = 0.0;

    for(int i=0; i<v.size();i++){
        suma += v.at(i);
    }
    return suma;
}
pair<vector<double>,double> Prim ( const vector<vector<double>> &matriz, int index){

    pair<vector<double>,double> sol;

    vector<double> aristas;
    vector<bool> vertices_seleccionados;
    int num_vertices = matriz.size();

    for(int i = 0; i< num_vertices; i++) vertices_seleccionados.push_back(false);

    vertices_seleccionados.at(index) = true;
    int pos_actual = index;

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

    aristas.back()= matriz.at(pos_actual).at(index);
    sol.first = aristas;
    sol.second = Suma_Distancias(aristas);


    return sol;

}


vector<double> Busca_Min_Recorrido(const vector<vector<double>> &matriz){
	
	pair<vector<double>,double> result = Prim(matriz,0);
   
    vector<double> min_recorrido = result.first;
    double min_dist = result.second;

    for(int i=1; i<matriz.size();i++){
        result = Prim(matriz,i);
        
        if(result.second<min_dist){
            min_dist = result.second;
            min_recorrido = result.first;
        }
    }

    return min_recorrido;
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
    sol = Busca_Min_Recorrido(matriz_dist);
    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << num_ciudades << "  " << transcurrido.count() << endl;

    return 0;
}
