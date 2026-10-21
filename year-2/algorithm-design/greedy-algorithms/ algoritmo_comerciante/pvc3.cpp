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


struct Arista{
    double distancia;
    int p1;
    int p2;
};



// Función que realiza la suma total de un vector de aristas
double Suma_Distancias(const vector<Arista> &v){

    double suma = 0.0;

    for(int i=0; i<v.size();i++){
        suma += v.at(i).distancia;
    }
    return suma;
}


// Función que busca un camino dado un grafo no dirigido y completo guardado en una matriz de distancias no negativas partiendo del nodo indicado por index
pair<vector<Arista>,double> Busca_Camino_Extension ( const vector<vector<Arista>> &matriz, int index){

    pair<vector<Arista>,double> sol;

    vector<Arista> aristas;
    vector<bool> vertices_seleccionados;
    int num_vertices = matriz.size();

    // Inicializamos todos los nodos a falso pues no han sido seleccionados aun
    for(int i = 0; i< num_vertices; i++) vertices_seleccionados.push_back(false);

    // Seleccionamos el nodo indicado en los parámetros
    vertices_seleccionados.at(index) = true;
    int pos_actual = index;

    while(aristas.size()<num_vertices){

        // Busco la arista mínima conectada a ese vértice
        double min = 9999999;
        int pos_min = pos_actual;

        for (int i = 0; i < num_vertices; ++i) {
            double d = matriz.at(pos_actual).at(i).distancia;
            if(d<min && d>0 && !vertices_seleccionados.at(i)){
                min = d;
                pos_min = i;
            }
        }

        // Marcamos como seleccionado el vértice con arista mínima
        vertices_seleccionados.at(pos_min) = true;

        // Añadimos el vértice con arista mínima a la solución
        aristas.push_back(matriz.at(pos_actual).at(pos_min));

        // Actualizamos la posción actual
        pos_actual = pos_min;
    }

    // Unimos el último nodo visitado con el nodo de partida
    aristas.back()= matriz.at(pos_actual).at(index);

    // Guardamos la solución
    sol.first = aristas;
    sol.second = Suma_Distancias(aristas); // calculamos la distancia total recorrida

    return sol;

}

// Funcion que busca el camino menor camino posible probando Buca_Camino pero empezando desde todos los nodos posibles
pair<vector<Arista>,double> Busca_Min_Camino(const vector<vector<Arista>> &matriz){

    pair<vector<Arista>,double> result = Busca_Camino_Extension(matriz,0);
    pair<vector<Arista>,double> min = result;
    /*
    cout << endl << "ARISTA 0: " << result.second << endl;
    for(int k=0; k<result.first.size();k++) {
        cout << result.first.at(k).distancia << endl;
    }
     */


    for(int i=1; i<matriz.size();i++){
        result = Busca_Camino_Extension(matriz,i);
        /*
        cout << "ARISTA " << i << ": " << result.second << endl;

        for(int k=0; k<result.first.size();k++) {
            cout << result.first.at(k).distancia << endl;
        }
         */
        if(result.second<min.second){
            min.second = result.second;
            min.first = result.first;
        }
    }

    return min;
}


// Función que dado un vector de aristas devuelve un vector con los vértices visitados en orden
vector<int> Crea_Camino(const vector<Arista> &aristas ){

    vector<int> camino_final;

    int num_vertices = aristas.size();

    for (int i=0; i< num_vertices; i++){
        camino_final.push_back(aristas.at(i).p1);
    }

    // Añadimos el vértice al que volvemos (en este caso era el de partida)
    camino_final.push_back(aristas.back().p2);

    return camino_final;
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
    vector<vector<Arista>> matriz_dist;

    // Lectura de la matriz de aristas
    if(fin){
        fin >> num_ciudades;

        vector<Arista> aux;
        for(int i = 0; i < num_ciudades; i++){
            for(int j = 0; j < num_ciudades;j++){
                double distancia;
                fin >> distancia;

                if(fin) {
                    Arista a = {distancia,i+1,j+1};
                    aux.push_back(a);
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

    /*
    for(int i=0; i< matriz_dist.size(); i++){

        cout << i << ": " ;
        //cout << endl;

        for(int j=0; j<matriz_dist.size(); j++){
            Arista a = matriz_dist.at(i).at(j);
            //cout << "Arista = { " << a.p1 << "," << a.p2 << ",";
            cout << setw(10);
            cout << a.distancia << "  ";
            //cout << "}"  << endl;
        }

        cout << endl;

    }
     */


    pair<vector<Arista>,double> sol;
    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    sol = Busca_Min_Camino(matriz_dist);
    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    cout << num_ciudades << "  " << transcurrido.count() << " ";
    cout << sol.second;
    cout << endl;

    /*
   //cout << endl << "ARISTAS" << endl;

   for(int k=0; k<sol.size();k++) {
       cout << sol.at(k).distancia << endl;
   }

]*/

    cout << endl << "VÉRTICES " << endl;

    vector<int> camino = Crea_Camino(sol.first);

    for(int k=0; k<camino.size();k++) {
        cout << camino.at(k) << "  ";
    }

    return 0;
}