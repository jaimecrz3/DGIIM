#include <cassert>
#include <ES.h>
#include <iostream>
#include <mddp.h>
#include <util.h>
#include <cmath>
#include <iostream>

using namespace std;

tSolution ES::neighborhood_op(const tSolution solucion_inicial, size_t size, size_t m){
    tSolution new_sol = solucion_inicial;

    tSolution no_seleccionados;
    tSolution aux_seleccionados(size, 0);
    for(int i=0; i<m; i++){
        aux_seleccionados[solucion_inicial[i]]=1;
    }
    for(int i=0; i<size; i++){
        if(aux_seleccionados[i]==0){
            no_seleccionados.push_back(i);
        }
    }

    int pos_out = Random::get(0,static_cast<int>(m - 1));
    int pos_in = Random::get(0,static_cast<int>(size - m - 1));
    tDomain elem_in = no_seleccionados[pos_in];

    new_sol[pos_out] = elem_in;

    return new_sol;
}

ResultMH ES::optimize(Problem *problem, const tSolution &current_param,tFitness fitness, int maxevals){
    assert(maxevals > 0);

    tSolution mejor_solucion;
    tFitness mejor_fitness;
    tSolution solucion_inicial;
    tFitness fitness_inicial;
    Problem *realproblem = problem;

    solucion_inicial = current_param;
    fitness_inicial = fitness;

    double u=0.2;
    double phi = 0.3;
    double ln_phi = log(phi);
    double t_inicial = (u*fitness_inicial)/-ln_phi;
    double temperature = t_inicial;
    double t_final = 1e-3;
    //comprobamos que tfinal < tinicial
    if(t_final >= t_inicial){
        t_inicial = t_final*10;
    }

    auto rango = realproblem->getSolutionDomainRange();
    unsigned size = rango.second+1;
    auto m = realproblem->getSolutionSize();

    const int max_vecinos = 100*m;
    const int max_exitos = 0.1*max_vecinos;

    double M = maxevals/max_vecinos;
    double beta = (t_inicial - t_final)/(M*t_inicial*t_final);

    mejor_solucion = solucion_inicial;
    mejor_fitness = fitness_inicial;

    int evaluaciones=0;
    bool hay_exitos = true;
    int nexitos, nvecinos;
    tSolution new_sol;
    tFitness new_fitness;
    tFitness delta;
    while(evaluaciones<maxevals && hay_exitos){

        nexitos=0;
        nvecinos=0;
        while(nexitos < max_exitos and nvecinos < max_vecinos and evaluaciones < maxevals){
            //modificamos un elemento
            new_sol = neighborhood_op(solucion_inicial, size, m);
            new_fitness = realproblem->fitness(new_sol);
            delta = new_fitness-fitness_inicial;

            nvecinos++;
            evaluaciones++;

            if(delta<0 or (Random::get<double>(0.0, 1.0)<=exp(-delta/temperature))){
                solucion_inicial = new_sol;
                fitness_inicial = new_fitness;
                nexitos++;

                if(fitness_inicial < mejor_fitness){
                    mejor_solucion = solucion_inicial;
                    mejor_fitness = fitness_inicial;
                }
            }
        }
        //actualizamos temperatura con el esquema de Cauchy modificado
        temperature = temperature/(1+(beta*temperature));

        if(nexitos==0){
            hay_exitos=false;
        }

    }

    return ResultMH(mejor_solucion, mejor_fitness, evaluaciones);

}