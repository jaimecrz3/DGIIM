#include <cassert>
#include <GRASP.h>
#include <iostream>
#include <mddp.h>
#include <util.h>
#include <algorithm>

using namespace std;

GRASP::GRASP(bool bl) : MH() {
  this->hay_bl = bl;
}

tSolution GRASP::greedy_aleatorizado(Mddp *realproblem, size_t size, size_t m){

    const double alpha = 0.2;
    tSolution sol_greedy;
    vector<int> LRC;
    vector<float> distancias_candidatos;
    vector<int> lista_candidatos;
    for (int i = 0; i < size; i++) {
        lista_candidatos.push_back(i);
   }
    if(m == 1){
        sol_greedy.push_back(lista_candidatos[0]);
    }else if(m == 2){
        sol_greedy.push_back(lista_candidatos[0]);
        sol_greedy.push_back(lista_candidatos[1]);
    }else{
        tDomain elem = Random::get(0, static_cast<int>(size - 1));
        sol_greedy.push_back(lista_candidatos[elem]);
        lista_candidatos.erase(lista_candidatos.begin() + elem);
        elem = Random::get(0, static_cast<int>(size - 2));
        sol_greedy.push_back(lista_candidatos[elem]);
        lista_candidatos.erase(lista_candidatos.begin() + elem);
        m-=2;
        while(m>0){
            //construir LRC
            for(int i=0; i<lista_candidatos.size(); i++){
                distancias_candidatos.push_back(realproblem->calcularDelta(sol_greedy, lista_candidatos[i]));
            }
            float min_distancia = *min_element(distancias_candidatos.begin(), distancias_candidatos.end());
            float max_distancia = *max_element(distancias_candidatos.begin(), distancias_candidatos.end());
            float umbral = min_distancia + alpha*(max_distancia-min_distancia);

            for(int i=0; i<lista_candidatos.size(); i++){
                if(distancias_candidatos[i]<=umbral){
                    LRC.push_back(lista_candidatos[i]);
                }
            }

            int index_in = Random::get(0, static_cast<int>(LRC.size()-1));
            sol_greedy.push_back(LRC[index_in]);
            lista_candidatos.erase(remove(lista_candidatos.begin(), lista_candidatos.end(), LRC[index_in]), lista_candidatos.end());
            LRC.clear();
            distancias_candidatos.clear();
            m--;
        }
    }

    return sol_greedy;
}

ResultMH GRASP::optimize(Problem *problem, int maxevals){
    assert(maxevals > 0);

    Mddp *realproblem = dynamic_cast<Mddp *>(problem); 
    const int ITERATIONS = 10;
    const int EVALS_BL = 10000;
    const int TOTAL_EVALS = 1;
    tSolution solucion_inicial;
    tFitness fitness_inicial;
    tSolution mejor_solucion;
    tFitness mejor_fitness;

    auto rango = realproblem->getSolutionDomainRange();
    unsigned size = rango.second+1;
    auto m = realproblem->getSolutionSize();

    for(int i=1; i<= ITERATIONS; i++){

        solucion_inicial = greedy_aleatorizado(realproblem, size, m);
        fitness_inicial = realproblem->fitness(solucion_inicial);

        if(hay_bl){
            ResultMH result = BL.optimize(realproblem, solucion_inicial, fitness_inicial, EVALS_BL);

            if(i==1){
                mejor_solucion = result.solution;
                mejor_fitness = result.fitness;
            }else if(result.fitness < mejor_fitness){
                mejor_solucion = result.solution;
                mejor_fitness = result.fitness;
            }
        }else{
            if(i==1){
                mejor_solucion = solucion_inicial;
                mejor_fitness = fitness_inicial;
            }else if(fitness_inicial< mejor_fitness){
                mejor_solucion = solucion_inicial;
                mejor_fitness = fitness_inicial;
            }
        }
    }

    return ResultMH(mejor_solucion, mejor_fitness, TOTAL_EVALS);

}