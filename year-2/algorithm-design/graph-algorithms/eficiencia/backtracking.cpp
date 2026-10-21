//
// Created by sarita on 27/04/23.
//
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;
using namespace std::chrono;

bool orden_decreciente(int i, int j){
    return i>j;
}

class Solucion {
private:
    const int NULO = -1;
    const int END =-2;
    int mejor_optimizacion;
    int num_invitados;
    int num_inv_sentados;
    vector<int> solucion;
    vector<int> mejor_solucion;
    vector<vector<int>> datos;
    vector<bool> estan_sentados;
    vector<int> nodos;

public:

    // Constructor con parámetros
    Solucion(const vector<vector<int>> &matriz) {

        datos = matriz;
        num_invitados=matriz.size();
        num_inv_sentados = 0;
        mejor_solucion = Greedy();
        // Inicializamos con conveniencia total de la mejor solución (al principio es Greedy)
        mejor_optimizacion=ConsigueSuma(mejor_solucion, mejor_solucion.size());

        // Ponemos todos los invitados de la solución a nulo (quiere decir que el sitio está libre)
        for(int i = 0; i < num_invitados; i++){
            solucion.push_back(NULO);
        }

        // Ponemos todos los sitios como libres
        for(int i = 0; i < num_invitados; i++){
            estan_sentados.push_back(false);
        }

        IniciaComp(0);
        SigValComp(0);
        estan_sentados.at(0) = true;

        // Inicializamos el vector de nodos
        for(int i = 0; i <num_invitados; i++){
            for(int j = 0; j < num_invitados; j++){
                nodos.push_back(datos.at(i).at(j));
            }
        }

        // Ordenamos el vector de nodos por conveniencia entre invitados
        sort(nodos.begin(), nodos.end(), orden_decreciente);
    }

    // Devuelve el número de invitados
    int getNumInvitados() const{
        return num_invitados;
    }

    // Devuelve la solución encontrada hasta ese momento
    vector<int> getSolucion() {
        return solucion;
    }

    // Devuelve la mejor solución encontrada
    vector<int> getMejorSolucion() {
        return mejor_solucion;
    }

    // Asigna el valor nulo a la posición k-ésima del vector solución
    void IniciaComp(int k) {
        solucion.at(k) = NULO;
    }

    // Asigna el siguiente valor válido de la posición k-ésima
    void SigValComp(int k) {

        int val = solucion.at(k);

        if (val < num_invitados -1) {
            if(val == NULO){
                num_inv_sentados++;
            }
            val++;
            solucion.at(k) = val;
        }
        else {
            solucion.at(k) = END;
        }
    }

    // Comprueba la posición k es el final de la solución
    bool TodosGenerados(int k) {
        return (solucion.at(k) == END);
    }


    // Comprueba si la solución obtenida es la mejor posible
    void ProcesaSolucion() {

        if (MejorSolucion()) {
            mejor_solucion = solucion;
            mejor_optimizacion = ConsigueSuma(mejor_solucion, mejor_solucion.size());
        }
    }

    //Devuelve "true" si la solución actual almacenada cumple con las restricciones y false en caso contrario
    bool Factible(int k, int cota) {
        bool es_factible = false;

        if (!estan_sentados.at(solucion.at(k))){
            int suma_local = CalculaCotaLocal(cota);

            if (suma_local > mejor_optimizacion) {
                es_factible = true;
                estan_sentados.at(solucion.at(k)) = true;
            }
        }

        return es_factible;
    }

    //Borra un invitado de los que están sentados
    void erase_invitado(int k){
        int val = solucion.at(k);
        estan_sentados.at(val) = false;
        num_inv_sentados--;
    }

   //Lanza el greedy para obtener una primera solución
   vector<int> Greedy() {
       vector<int> sol;
       int suma = 0;
       vector<bool> usados;
       int num_invitados = datos.size();


       for (int i = 0; i < num_invitados; i++) {
           usados.push_back(false);
       }

       int invitado_actual = 0;
       sol.push_back(0);
       usados.at(0) = true;
       while (sol.size() < num_invitados) {

           int max = -1;
           int pos_max = invitado_actual;

           for (int i = 0; i < num_invitados; ++i) {
               int c = datos.at(invitado_actual).at(i);
               if (c > max && c>0 && !usados.at(i)) {
                   max = c;
                   pos_max = i;
               }
           }

           usados.at(pos_max) = true;
           sol.push_back(pos_max);
           invitado_actual = pos_max;
       }

       /*
       suma = ConsigueSuma(sol, sol.size());

       for(int x=0; x<sol.size();x++){
           cout << sol.at(x) << " ";
       }
       cout << ". TOTAL: " << suma << endl;
        */

       return sol;
   }



private:

    // Comprueba si la solución actual es mejor que la hasta ahora conseguida
    bool MejorSolucion() {
        int suma = ConsigueSuma(solucion, solucion.size());
        return (suma > mejor_optimizacion);
    }

    //Calcula la cota local asociada a un nodo; es decir la mejor solución que se podría obtener al expandir un nodo
    int CalculaCotaLocal(int cota){

        int cotalocal = DecisionesTomadas();

        if(cota==1){
            cotalocal += Estimador1();
        }
        else{
            if(cota==2)
                cotalocal += Estimador2();
            else
                cotalocal += Estimador3();
        }

        return cotalocal;
    }

    // Devuelve el valor de las decisiones ya tomadas desde un nodo
    int DecisionesTomadas(){
        return ConsigueSuma(solucion, num_inv_sentados);
    }

    /*  Estima el mejor valor que se podría alcanzar al expandir un nodo, encontrando la mayor conveniencia de los nodos
        que quedan y multiplicando dicho peso por los invitados que quedan
     */
    int Estimador1(){

        int invitados_sin_sentar= num_invitados - num_inv_sentados;
        int estimador=0;

        if(num_inv_sentados>0){
            estimador = nodos.front();
        }

        return 2*estimador*invitados_sin_sentar;
    }

    int Estimador2(){
        int estimador2 = 0;
        if(num_inv_sentados > 0) {
            int tam = getNumInvitados();

            for (int i = 0; i < tam; i++) {
                if (!estan_sentados.at(i) && i != solucion.at(num_inv_sentados - 1)) {
                    vector<int>::iterator it = max_element(datos.at(i).begin(), datos.at(i).end());
                    estimador2 += 2 * (*it);
                }
            }
        }
        return estimador2;
    }

    int Estimador3(){
        int estimador3 = 0;
        int max = 0;
        if(num_inv_sentados>0) {
            int tam = getNumInvitados();
            for (int i = 0; i < tam; i++) {
                max = 0;
                if (!estan_sentados.at(i) && i != solucion.at(num_inv_sentados - 1)) {
                    // Buscamos la conveniencia máxima de cada fila
                    int indice_max;
                    for (int j = 0; j < datos.at(i).size(); j++) {

                        if (datos.at(i).at(j) > max){
                            max = datos.at(i).at(j);
                            indice_max = j;
                        }

                    }

                    // Buscamos el segundo máximo de esa misma fila
                    int otro_max = 0;
                    for (int j = 0; j < datos.at(i).size(); j++) {

                        if (datos.at(i).at(j) > otro_max && indice_max != j)
                            otro_max = datos.at(i).at(j);
                    }

                    // La máxima conveniencia de cada inivitado estará compuesta por la suma de los dos máximos de su fila
                    max += otro_max;
                }

                estimador3 += max;
            }
        }

        return estimador3;
    }

    // Calcula la conveniencia que hay con una disposición concreta de los comensales
    int ConsigueSuma(const vector<int> &v, int tam) {

        int suma = 0;

        if(tam > 1) {

            for (int i = 0; i < tam; i++) {

                int val = v.at(i);

                if (i == 0) {
                    suma += datos.at(val).at(v.at(i + 1));
                    suma += datos.at(val).at(v.at(tam - 1));

                }
                else{
                    if (i < (tam - 1)) {
                        suma += datos.at(val).at(v.at(i + 1));
                        suma += datos.at(val).at(v.at(i - 1));

                    }
                    else {
                        suma += datos.at(val).at(v.at(0));
                        suma += datos.at(val).at(v.at(i - 1));
                    }
                }
            }
        }

        return suma;
    }

};


void back_recursivo(Solucion &s, int k, int cota){

    if(k==s.getNumInvitados()){
        s.ProcesaSolucion();
    }else{
        s.IniciaComp(k);
        s.SigValComp(k);
        while(!s.TodosGenerados(k)){
            if(s.Factible(k,cota)){
                back_recursivo(s, k+1,cota);
                s.erase_invitado(k);
            }

            s.SigValComp(k);
        }
    }
}
int main(int argc, char *argv[]){
	
	if (argc != 3 ){
		cerr << "Formato " << argv[0] << " <fichero_conveniencias> <num_cota: 1,2,3>" << endl;
		return -1;
	}

   	char *fich = argv[1];
   	int cota = atoi(argv[2]);
	
	if (cota != 1 && cota != 2 && cota != 3) {
        cout << "ERROR: Cota a usar debe ser: 1, 2, 3" << endl;
        return -1;
    }
    
	ifstream fin;
	fin.open(fich);
	int num_invitados;
	vector<vector<int>> datos;

	// Lectura de la matriz de aristas
	if(fin) {
	   fin >> num_invitados;

	   vector<int> aux;
	   for (int i = 0; i < num_invitados; i++) {
		   for (int j = 0; j < num_invitados; j++) {
		       int conveniencia;
		       fin >> conveniencia;
		       if (fin) {
		           aux.push_back(conveniencia);
		       }
		   }
		   datos.push_back(aux);
		   aux.clear();
	   }
	}
	else{
	   cout << "ERROR: no se puedo abrir el fichero " << argv[1] << endl;
	   return -1;
	}
	/*

	for(int i = 0; i < num_invitados; i++){
		for(int j = 0; j < num_invitados; j++){
		    cout << datos.at(i).at(j) << "   " ;
		}
		cout << endl;
	}
	cout << endl;
	*/

	Solucion sol(datos);

	high_resolution_clock::time_point t_antes,t_despues;
	duration<double> transcurrido;

	t_antes = high_resolution_clock::now();
	back_recursivo(sol,1,cota);
	t_despues = high_resolution_clock::now();

	transcurrido = duration_cast<duration<double>> (t_despues - t_antes);

	/*
	cout << endl << " SOLUCION "  << endl;
	for(int i = 0; i < sol.getNumInvitados(); i++){
		cout << sol.getMejorSolucion().at(i) << "   " ;
	}

	cout << endl;
	cout << "Cota " << cota << endl;
	cout << "Tiempo: " << transcurrido.count() << endl;
	*/
	cout << num_invitados << "  " << transcurrido.count() << endl;
	
	return 0;

	}




