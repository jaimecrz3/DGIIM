//
// Created by sarita on 18/04/23.
//

#include<vector>
#include<iostream>
#include <fstream>
#include<algorithm>
#include<set>
#include<list>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;
using namespace std::chrono;

/**
 * Estructura para identificar cada vértice, en concreto para este problema cada una de las ciudades
 * @param id identificador de cada ciudad, no puede haber dos ciudades con dos id iguales
 * Operadores relacionales
 */
struct Vertice{
    int id;
    bool operator == (Vertice p) const{
        return (id == p.id);
    }
    bool operator != (Vertice p) const{
        return (id != p.id);
    }

    bool operator < (Vertice p) const{
        return (id > p.id);
    }
};
/**
 * Estructura para describir una arista; es decir, el camino entre dos ciudades
 * @param distancia Indica la longitud del camino
 * @param punto1 Indica la primera ciudad que une la arista
 * @param punto2 Indica la segunda ciudad que une la arista
 * Operador relacional
 */
struct Arista{
    double distancia;
    Vertice punto1;
    Vertice punto2;
    bool operator<(Arista p) const{
        return (distancia < p.distancia);
    }

};
/**
 * La función se va a encargar de encontrar la componente conexa a la que pertenece cada ciudad
 * @param componentes Conjunto de las componentes conexas donde se tiene que buscar
 * @param p Ciudad que se quiere buscar en la componente conexa
 * @return Devuelve la componente conexa a la que pertenence la ciudad y se puede asegurar de que siempre existe,
 * pues al inicio el conjunto de componentes conexas es inicializado con todas las componentes.
 */

set<Vertice> findSet( set<set<Vertice>>  & componentes, const Vertice & p){
    bool encontrado= false;
    set<set<Vertice>>::iterator it = componentes.begin();
    set<Vertice> resultado;
    while(it != componentes.end() && !encontrado){
        set<Vertice>::iterator iter;
        iter = (*it).find(p);
        if(iter != (*it).end()){
            encontrado = true;
            resultado = *it;
        }
        else ++it;
    }

    return resultado;
}

/**
 * La funcion Union va a realizar la union de dos componenentes conexas, se sabe que la interseccion es vacío,
 * luego va añadir el contenido de una de las componenets conexas en la otra
 * @param setU Primera componente conexa que se quiere fusuonar
 * @param setV Segunda componente conexa que se quiere fusuonar
 * @return Devueleve la union de las dos comprnenets conexas
 */

set<Vertice> Union(set<Vertice> setU, set<Vertice> setV){
    set<Vertice>:: iterator it;

    for(it = setV.begin(); it != setV.end(); ++it) setU.insert(*it);

    return setU;

}
/**
 * Comprueba que dos componentes conexas son distintas, como estas son disjuntas comprobando que uno de los elementos es suficiente
 * @param setA Componente conexa
 * @param setB Componente conexa
 * @return Devuelve true si las componentes conexas son distintas y false si son iguales
 */
bool setDifferent(set<Vertice> setA, set<Vertice> setB){

    set<Vertice>::iterator it = setA.begin();
    set<Vertice>::iterator iter;
    bool son_diferentes =true;

    iter = setB.find(*it);
    if(iter != setB.end())  son_diferentes = false;


    return son_diferentes;
}
/**
 * Comprueba que desde un vértice no salgan mas de dos aristas; es decir, para cada ciudad unida por el camino, verifica que no se pasa por ellas mas de una vez
 * @param vertices Vector donde se almacena el numerode aristas que salen de cada vertice
 * @param arista Arista de la cual obtenemos los vertices que queremos comprobar
 * @return Devuelve true si desde alguno de los dos vertices salen dos aristas  y false si desde ninguno salen dos aristas.
 */
bool UnVerticeDosAristas(vector<int> vertices, Arista arista){
    bool un_vertice_tiene_dos_aristas = false;

    if(vertices.at(arista.punto1.id -1) ==2 || vertices.at(arista.punto2.id -1) ==2 ) un_vertice_tiene_dos_aristas=true;
    return un_vertice_tiene_dos_aristas;
}

/**
 * Se trata del algoritmo que resuelve el problema del viajante de comercio baandose en la idea de kruskal
 * @param vertices Vector de vertices, en este caso ciudades con los que se trabaja
 * @param aristas Vector de list de aristas, dodne cada arista guarad su longitud y las dos ciudades que une, sin repeticiones
 * @return Devuelve el conjunto de aristas de salida que indican el camino a seguir
 */
vector<Arista> PVC1(const vector<Vertice> & vertices, vector<list<Arista>> & aristas){

    vector<list<Arista>> aristas_ordenadas = aristas;
    //Crear un set con cada vertice, cada componente conexa
    int n = vertices.size();
    set<set<Vertice>> componentes;

    // Crear el conjunto de la componentes conexas, inicialmente cada vértice en una componente conexa distinta
    for(int i = 0; i < vertices.size(); i++){
        set<Vertice> conjunto;
        conjunto.insert(vertices.at(i));
        componentes.insert(conjunto);

    }

    // Ordenar cada lista de aristas

    vector<list<Arista>>::iterator it;
    for(it= aristas_ordenadas.begin(); it != aristas_ordenadas.end(); ++it){
        (*it).sort();
    }

    vector<Arista> conjunto_salida;
    // Creamos el vector conatdor del numero de aristas que salen de cada vertice, inicializando todas las posiciones a 0
    vector<int> vertices_aristas_salientes;
    for(int i = 0; i < aristas_ordenadas.at(0).size()+1; i++) vertices_aristas_salientes.push_back(0);

    // Mientras el numero de aritas en el vector de salida menor que el numero de vertices menos uno pues la ultiam arista se mete de forma manuela para cerrar el ciclo
    while(conjunto_salida.size() < n-1 && !aristas_ordenadas.empty()){

        //Sacar la arista minima

        Arista minima = aristas_ordenadas.at(0).front();

        int i = 0;
        int num =0;
        vector<list<Arista>>::iterator it= aristas_ordenadas.begin();
        vector<list<Arista>>::iterator fila = it;

        // Ver si es la arista minima
        for(it = aristas_ordenadas.begin();it != aristas_ordenadas.end(); ++it){

            if((*it).front() < minima ) {
                num = i;
                fila = it;
                minima = (*it).front();
            }
            i++;
        }
        // Eliminar la arista candidate del conjunto de candidatos
        (*fila).pop_front();
    // Si alguna fila se queda vacia borrarlas
        if(aristas_ordenadas.at(num).empty()) aristas_ordenadas.erase(fila);

        // Buscar en que componentes estan los vertices la arista seleccionada

        Vertice u = minima.punto1;
        Vertice v = minima.punto2;

        set<Vertice> setU = findSet(componentes, u);
        set<Vertice> setV = findSet(componentes, v);

        // Aplicar la funcion de factibilidad; es decir, elegir dicho cnadidato nos garantiza alcanzar la solucion
        if (!UnVerticeDosAristas(vertices_aristas_salientes, minima) && setDifferent(setU,setV)) {
            // Aumentamos el numero de aristas salientes de cada ciudad
            vertices_aristas_salientes.at(minima.punto1.id - 1)++;
            vertices_aristas_salientes.at(minima.punto2.id - 1)++;
            // Introducimos la candidata en el conjunto solución
            conjunto_salida.push_back(minima);

            // Eliminamos las componentes conexas de cada vertice y añadimos la union de ambas
            componentes.erase(setU);
            componentes.erase(setV);
            set<Vertice> nuevo = Union(setU, setV);

            componentes.insert(nuevo);
        }

    }
    // Buscamos los dos vertices que quedan por unir, aquellos que tengan tan solo uan arista saliente
    Vertice u;
    Vertice v;

    int id1 ;
    bool encontrado = false;
    int i = 0;
    while (i < vertices_aristas_salientes.size() && !encontrado){
        if(vertices_aristas_salientes.at(i) == 1){
            id1 = i+1;
            encontrado = true;
            vertices_aristas_salientes.at(i)++;
        }
        else i++;
    }
    u.id = id1;

    int id2 ;
    encontrado = false;
    i = 0;
    while (i < vertices_aristas_salientes.size() && !encontrado){
        if(vertices_aristas_salientes.at(i) == 1){
            id2 = i+1;
            encontrado = true;
            vertices_aristas_salientes.at(i)++;
        }
        else i++;
    }
    v.id = id2;

    Arista ultima;
    // Creamos la arista final
    if(u.id < v.id){
        ultima.punto1 = u;
        ultima.punto2 = v;
        list<Arista>::iterator it= aristas.at(u.id -1).begin();
        bool encontrado = false;
        while( it != aristas.at(u.id -1).end() && !encontrado){
            if((*it).punto2.id == v.id){
                ultima.distancia = (*it).distancia;
                encontrado = true;
            }
            else ++it;
        }
    }
    else{
        ultima.punto1 = v;
        ultima.punto2 = u;
        list<Arista>::iterator it= aristas.at(v.id -1).begin();
        bool encontrado = false;
        while( it != aristas.at(v.id -1).end() && !encontrado){
            if((*it).punto2.id == u.id){
                ultima.distancia = (*it).distancia;
                encontrado = true;
            }
            else ++it;

        }
    };
    // Añadimos la ultima arista al vector solucion
    conjunto_salida.push_back(ultima);

    return conjunto_salida;


}
/**
 * Función que muestra el camino calculado por el algoritmo PVC1
 * @param aristas Vector de aristas, resultado del algoritmo PVC1; es decir, conjunto de aristas que conforman el camino
 * @return Devuelve un vector de eneteros con el orden en que se deben recorrer las ciudades para cerrar el ciclo
 */
vector<int> MostrarCamino( vector<Arista> &aristas ){

    vector<int> camino_final;

    Arista a =aristas.front();
    camino_final.push_back(a.punto1.id);
    camino_final.push_back(a.punto2.id);
    Vertice v = a.punto2;
    aristas.erase(aristas.begin());
    while(!aristas.empty()){
        bool encontrado = false;
        vector<Arista>::iterator it;
        for(it=aristas.begin(); it != aristas.end() && !encontrado; ++it){
            if((*it).punto1==v || (*it).punto2==v){
                encontrado = true;
                if((*it).punto1==v ){
                    camino_final.push_back((*it).punto2.id);
                    v= (*it).punto2;
                }
                else {
                    camino_final.push_back((*it).punto1.id);
                    v = (*it).punto1;
                }
                aristas.erase(it);
            }
        }
    }


    return camino_final;
}
int main(int argc, char* argv[]) {


    if (argc != 2){
        cerr << "Formato: " << argv[0] << " <fichero_puntos>" << endl;
        return -1;
    }

    char *nom_fich= argv[1];

    ifstream fin;
    fin.open(nom_fich);
    int num_puntos;
    vector<list<Arista>> puntos;
    list<Arista> aux;

    // Leemos desde fichero el numeor de ciudades y las sitancias entre ellas
    fin >> num_puntos;

    if(fin){
        // NO se toman los datos repetidos, si los representasemos en una matriz nos quedamos con la triangular superior
        for(int i = 0; i < num_puntos-1; i++){
            for(int j = 0; j < num_puntos ; j++){
                double distancia;
                fin >> distancia;
                if(i< j && fin){

                    Arista nueva={distancia, i+1, j+1};
                    aux.push_back(nueva);
                }
            }
            puntos.push_back(aux);
            aux.clear();
        }
        fin.close();
    }
    else{
        cout << "ERROR: no se ha podido abrir el fichero que contiene los puntos." << endl;
        return -1;
    }

    vector<Vertice> vertices;
    // Creamos el ocnjunto de ciudades
    for(int i =0; i < num_puntos; i++){
        Vertice v={i+1};
        vertices.push_back(v);
    }

    // Mostramos las ciudades y las distancias entre ellas
    vector<list<Arista>>::iterator  its;
    for(its=puntos.begin(); its != puntos.end(); ++its){
        list<Arista>::iterator iter;
        for(iter=(*its).begin(); iter != (*its).end(); ++iter){
            cout << setw(10 ) << (*iter).distancia << " ";
        }
        cout << endl;
    }
    cout << endl;


    // Se llama al algoritmo
    high_resolution_clock::time_point t_antes,t_despues;
    duration<double> transcurrido;

    t_antes = high_resolution_clock::now();
    vector<Arista> salida = PVC1(vertices, puntos);
    t_despues = high_resolution_clock::now();

    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

    // cout << num_puntos << "  " << transcurrido.count() << endl;
    vector<Arista>::iterator it;

    int coste = 0;
    for(it = salida.begin(); it != salida.end(); ++it){

        coste +=(*it).distancia;
    }
    vector<int> sol;
    // Se muestra la solucion
    sol=MostrarCamino(salida);
    cout << "Orden ciudades: ";
    for(int i = 0; i < sol.size(); i++){
        cout<< " -> " << sol.at(i) ;
    }
    cout << endl;
    cout << num_puntos << "  " << transcurrido.count() << "  " << coste << endl;

    return 0;
}
