#include <cassert>
#include <BMB.h>
#include <iostream>
#include <mddp.h>
#include <util.h>

using namespace std;

ResultMH BMB::optimize(Problem *problem, int maxevals){
    assert(maxevals > 0);

    Problem *realproblem = problem;
    const int ITERATIONS = 10;
    const int EVALS_BL = 10000;
    const int TOTAL_EVALS = 1;
    tSolution mejor_solucion;
    tFitness mejor_fitness;
    tSolution solucion_inicial;
    tFitness fitness_inicial;


    for(int i=1; i<= ITERATIONS; i++){

        solucion_inicial = realproblem->createSolution();
        fitness_inicial = realproblem->fitness(solucion_inicial);

        ResultMH result = BL.optimize(realproblem, solucion_inicial, fitness_inicial, EVALS_BL);

        if(i==1){
            mejor_solucion = result.solution;
            mejor_fitness = result.fitness;
        }else if(result.fitness < mejor_fitness){
            mejor_solucion = result.solution;
            mejor_fitness = result.fitness;
        }

    }

    return ResultMH(mejor_solucion, mejor_fitness, TOTAL_EVALS);

}