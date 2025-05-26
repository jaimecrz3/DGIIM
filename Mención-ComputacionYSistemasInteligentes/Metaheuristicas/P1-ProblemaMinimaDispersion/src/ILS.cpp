#include <cassert>
#include <ILS.h>
#include <iostream>
#include <mddp.h>
#include <util.h>

using namespace std;

void ILS::mutacion(tSolution &solucion_inicial, tSolution &no_seleccionados ,const int SIZE_MUTATION){

    //Creamos matriz con todas las posiblidades de intercambio
    vector<vector<pair<int, int>>> posibilidades;
    for(int i=0; i<solucion_inicial.size(); i++){
        vector<pair<int, int>> aux;
        for(int j=0; j<no_seleccionados.size(); j++){
            aux.push_back({i, j});
        }
        posibilidades.push_back(aux);
        Random::shuffle(posibilidades[i]);
    }

    Random::shuffle(posibilidades);

    int cont=0;
    bool ok=true;
    for(int i=0; i<posibilidades.size() and ok; i++){
        for(int j=0; j<posibilidades[i].size() and ok; j++){
            int pos_out = posibilidades[i][j].first;
            int pos_in = posibilidades[i][j].second;

            tDomain elem_out = solucion_inicial[pos_out];
            tDomain elem_in = no_seleccionados[pos_in];
            solucion_inicial[pos_out] = elem_in;
            no_seleccionados[pos_in] = elem_out;

            cont++;
            if(cont>=SIZE_MUTATION){
                ok=false;
            }

        }
    } 

}

ResultMH ILS::optimize(Problem *problem, int maxevals){
    assert(maxevals > 0);

    Problem *realproblem = problem;
    auto rango = realproblem->getSolutionDomainRange();
    unsigned size = rango.second+1;
    auto m = realproblem->getSolutionSize();

    const int ITERATIONS = 9;
    const int EVALS_BL = 10000;
    const int TOTAL_EVALS = 1;
    const int MUTATION=0.2;
    int SIZE_MUTATION = MUTATION*m;
    if(SIZE_MUTATION<2){
        SIZE_MUTATION=2;
    }

    tSolution mejor_solucion;
    tFitness mejor_fitness;
    tSolution solucion_inicial;
    tFitness fitness_inicial;

    solucion_inicial = realproblem->createSolution();
    fitness_inicial = realproblem->fitness(solucion_inicial);

    ResultMH result = BL.optimize(realproblem, solucion_inicial, fitness_inicial, EVALS_BL);

    mejor_solucion = result.solution;
    mejor_fitness = result.fitness;

    tSolution no_seleccionados;
    tSolution aux_seleccionados(size, 0);
    for(int i=0; i<m; i++){
        aux_seleccionados[mejor_solucion[i]]=1;
    }
    for(int i=0; i<size; i++){
        if(aux_seleccionados[i]==0){
            no_seleccionados.push_back(i);
        }
    }

    for(int i=1; i<= ITERATIONS; i++){

        //mutacion: escogemos un 20% de los
        //elementos, para provocar un cambio brusco, mínimo 2 elementos.
        mutacion(solucion_inicial, no_seleccionados, SIZE_MUTATION);
        fitness_inicial = realproblem->fitness(solucion_inicial);

        ResultMH result = BL.optimize(realproblem, solucion_inicial, fitness_inicial, EVALS_BL);

        if(result.fitness < mejor_fitness){
            mejor_solucion = result.solution;
            mejor_fitness = result.fitness;
        }

        solucion_inicial = mejor_solucion;
        fitness_inicial = mejor_fitness;

    }

    return ResultMH(mejor_solucion, mejor_fitness, TOTAL_EVALS);

}