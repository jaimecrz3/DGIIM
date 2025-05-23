#include "BLheur.h"
#include <cassert>
#include <iostream>
#include <util.h>
#include <mddp.h>
#include <algorithm>

using namespace std;

/*
Representación: Un conjunto Sel={s1,s2,...,sm} que almacena los m elementos seleccionados
entre los n posibles elementos del problema.
Paa ser una solucioń candidata válida, 
    1. Sel debe contener exactamente m elementos.
    2. No puede tener elementos repetidos.
    3. El orden de los elementos en Sel no importa.

Busqueda Local del Primer Mejor:
Toma el primer movimiento que mejora la solución actual, sin importar si hay otros movimientos que mejoren más la solución.
Mediante una exploración del entorno que puede ser:
    1. Aleatoria(randLS)
    2 guiada por huristica(heurLS)

-> Factorización de la fución objetivo:
Solo actualiza la parte afectada por el cambio, evitando calculos innecesarios.

-> Movimientos: Intercambio de un elemento de Sel por un elemento no seleccionado.

-> Fucion objetivo: Calcular la minima dspersión. La dispersion se calcula como: 
    1. Para cada punto elegido v calcula delta(v) como la suma de las distancias de v a los puntos elegidos.
    2. La disperson de una solucion, denotada cmoo diff(S), se define como la diferencia entre los valores extremos 
         es decir, la diferencia entre el maximo y el minimo de los valores delta(v) para v en S.
    3. El objetivo es minimizar la dispersion de la solucion.
*/



//El algoritmo de primero mejor no tiene forma de escapar de los optimos locales
//Salvo al ejecutarlo con distintas semillas
ResultMH BusquedaLocalMejor::optimize(Problem *problem, const int maxevals) {
  
   assert(maxevals > 0);
//    Mddp *realproblem = dynamic_cast<Mddp *>(problem);
    Problem *realproblem = problem;

   //Inicializamos una solucion aletoria
    tSolution seleccionados;
    vector<int> no_seleccionados;
    auto rango = realproblem->getSolutionDomainRange();
    unsigned size = rango.second;

    vector<int> aux(size+1);
    for(int i=0; i<aux.size(); i++){
        aux[i] = i+rango.first;
    }
    Random::shuffle(aux);       

    for(int i=rango.first; i<= rango.second; i++){
        if(i<realproblem->getSolutionSize()){
            seleccionados.push_back(aux[i]);
        }else{
            no_seleccionados.push_back(aux[i]);
        }
    }

    SolutionFactoringInfo *info = realproblem->generateFactoringInfo(seleccionados);
    tFitness mejor_fitness = realproblem->fitness(seleccionados);
    int evaluaciones=1;
    bool mejora = true;
    while(evaluaciones <= maxevals && mejora){

        mejora = false;

        //Con el fitness factorizado, vamosa encontrar el mejor movimiento posible, si existiese alguno
        //que mejora la solucion actual, si no, estamos en un optimo local, 
        int pos_in;
        int pos_out;
        tFitness mejor_fitness_aux = mejor_fitness;
        for(int i=0; i<seleccionados.size(); i++){
            for(int j=0; j<no_seleccionados.size(); j++){
                tDomain elem_in = no_seleccionados[j];
                tFitness nuevo_fitness = realproblem->fitness(seleccionados, info, i, elem_in);
                if(nuevo_fitness < mejor_fitness_aux) {
                    mejor_fitness_aux = nuevo_fitness;
                    pos_out = i;
                    pos_in = j;
                    mejora = true;
                }
            }

        }

        
        if(mejora){
            evaluaciones++;
            mejor_fitness = mejor_fitness_aux;
            tDomain elem_out = seleccionados[pos_out];
            tDomain elem_in = no_seleccionados[pos_in];
            realproblem->updateSolutionFactoringInfo(info, seleccionados, pos_out, elem_in);
            seleccionados[pos_out] = elem_in;
            no_seleccionados[pos_in] = elem_out;
        
        }


    }

    delete info;

  return ResultMH(seleccionados, mejor_fitness, evaluaciones);
}

