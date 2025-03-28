#include <cassert>
#include <greedy.h>
#include <iostream>
#include <mddp.h>
#include <util.h>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 * 
 * 
 * Podemos determinar que un buena formula heuristica es la siguiente:
 *
 * Añadir secuencialmente el elemento no seleccionado que reduzca la dispersion con respecto a los ya seleccionados.
 * -> El algoritmo valora como varia la dispersion al seleccionar cada nuevo elemento
 * -> Eñ primer elemento puede ser aleatorio
 * -> Cada vez que se añade un nuevo elemento al conjunto Sel, se valora cual incrementa menos la dispersion respecto a los ya elegidos
 * -> El proceso itera hasta seleccionar los m elementos deseados
 * 
 * Se realizara una unica ejecucion sobre cada uno de los 50 casos del problema
 * 
 */
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  vector<tDomain> values;
  Mddp *realproblem = dynamic_cast<Mddp *>(problem); 
  tSolution sol;
  int m  = realproblem->getSolutionSize();
  tFitness fitness;

  auto size = realproblem->getProblemSize();

  for (int i = 0; i < size; i++) {
    values.push_back(i);
  }

  if(realproblem->getSolutionSize() == 1){
    sol.push_back(values[0]);
    fitness = realproblem->fitness(sol);
    return ResultMH(sol, fitness, 1);
  }else if(realproblem->getSolutionSize() == 2){
    sol.push_back(values[0]);
    sol.push_back(values[1]);
    fitness = realproblem->fitness(sol);
    return ResultMH(sol, fitness, 1);
  }else{
    //sol.push_back(values[0]);
    //values.erase(values.begin());
    tDomain elem = Random::get(0, static_cast<int>(size - 1));
    sol.push_back(values[elem]);
    values.erase(values.begin() + elem);
    int index = Random::get(0, static_cast<int>(values.size() - 1));
    sol.push_back(values[index]);
    values.erase(values.begin() + index);
    m-=2;
    while(m>0){
      tSolution sol_copy(sol);
      index = 0;
      sol_copy.push_back(values[index]);
      fitness = realproblem->fitness(sol_copy);
      for(int i = 1; i < values.size(); i++){
        sol_copy.pop_back();
        sol_copy.push_back(values[i]);
        float new_fitness = realproblem->fitness(sol_copy);
        if(new_fitness < fitness){
          index = i;
          fitness = new_fitness;
        }
      }

      sol.push_back(values[index]);
      values.erase(values.begin() + index);

      m--;
    }
  }

  return ResultMH(sol, realproblem->fitness(sol), 1);
}

