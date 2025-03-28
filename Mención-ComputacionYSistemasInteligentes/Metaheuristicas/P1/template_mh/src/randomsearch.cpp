#include <cassert>
#include <randomsearch.h>

using namespace std;

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH RandomSearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  tSolution best;
  tFitness best_fitness = -1;

  for (int i = 0; i < maxevals; i++) {    //¿NO HAY QUE GENERAR 100000 SOLUCIONES? EN EL MAIN SE PASA maxevals=100
    tSolution solution = problem->createSolution();
    tFitness fitness = problem->fitness(solution);

    if (fitness < best_fitness || best_fitness < 0) {
      best = solution;
      best_fitness = fitness;
    }
  }

  return ResultMH(best, best_fitness, maxevals);
}
