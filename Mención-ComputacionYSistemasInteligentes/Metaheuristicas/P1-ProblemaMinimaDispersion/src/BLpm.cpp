#include "BLmejor.h"
#include <cassert>
#include <iostream>
#include <util.h>
#include <mddp.h>
#include <algorithm>

using namespace std;

//
ResultMH BusquedaLocalMejor::optimize(Problem *problem, const int maxevals) {

   assert(maxevals > 0);
   Mddp *realproblem = dynamic_cast<Mddp *>(problem);

   //Inicializamos una solucion aletoria
    tSolution seleccionados;
    vector<int> no_seleccionados;

    vector<int> aux(realproblem->getProblemSize());
    for(int i=0; i<aux.size(); i++){
        aux[i] = i;
    }
    Random::shuffle(aux);       

    for(int i=0; i<realproblem->getProblemSize();i++){
        if(i<realproblem->getSolutionSize()){
            seleccionados.push_back(aux[i]);
        }else{
            no_seleccionados.push_back(aux[i]);
        }
    }

    MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(realproblem->generateFactoringInfo(seleccionados));
    tFitness mejor_fitness = realproblem->fitness(seleccionados);
    int evaluaciones=1;
    bool mejora = true;

    while(evaluaciones <= maxevals && mejora){

        mejora=false;

        //Creamos matriz con todas las posiblidades de intercambio
        //
        vector<vector<pair<int, int>>> posibilidades;
        for(int i=0; i<seleccionados.size(); i++){
            vector<pair<int, int>> aux;
            for(int j=0; j<no_seleccionados.size(); j++){
                aux.push_back({i, j});
            }
            posibilidades.push_back(aux);
            //La exploración inteligente del vecindario se hace en orden aleatorio
            Random::shuffle(posibilidades[i]);
        }

        //La exploración inteligente del vecindario se hace en orden aleatorio
        Random::shuffle(posibilidades);

        for(int i=0; i<posibilidades.size(); i++){
            for(int j=0; j<posibilidades[i].size(); j++){
                int pos_out = posibilidades[i][j].first;
                int pos_in = posibilidades[i][j].second;

                //Calculamos el fitness de la solución con el intercambio
                tFitness nuevo_fitness = realproblem->fitness(seleccionados, info, pos_out, no_seleccionados[pos_in]);

                //Si mejora, actualizamos la solución
                if(nuevo_fitness < mejor_fitness){
                    realproblem->updateSolutionFactoringInfo(info, seleccionados, pos_out, no_seleccionados[pos_in]);
                    tDomain elem_out = seleccionados[pos_out];
                    tDomain elem_in = no_seleccionados[pos_in];
                    seleccionados[pos_out] = elem_in;
                    no_seleccionados[pos_in] = elem_out;
                    mejor_fitness = nuevo_fitness;
                    mejora = true;
                }
                if (mejora) break;
            }
            if (mejora) break;
        }

        evaluaciones++;
        
    }

    return ResultMH(seleccionados, mejor_fitness, evaluaciones);

}