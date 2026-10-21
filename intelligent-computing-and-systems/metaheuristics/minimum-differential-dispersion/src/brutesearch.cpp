#include "brutesearch.h"
#include <cassert>
#include <iostream>
#include <util.h>

using namespace std;

ResultMH BruteSearch::optimize(Problem *problem, const int maxevals) {
  tSolution solution(problem->getSolutionSize());
  tFitness fitness, best_fitness;
  size_t size = solution.size();
  assert(maxevals > 0);
  fitness = problem->fitness(solution);
  tSolution best_solution = solution;
  best_fitness = fitness;

  for (int i = 1; i < maxevals; i++) {
    unsigned int accu = 1;
    int posi = size - 1;

    // Get the following solution
    while (accu > 0) {
      if (!solution[posi]) {
        solution[posi] = true;
        accu = 0;
      } else {
        solution[posi] = false;
        posi = posi - 1;

        if (posi < 0) {
          return ResultMH(best_solution, best_fitness, i);
        }
      }
    }

    // cout << "Solution: " << solution << endl;
    // Eval the fitness
    fitness = problem->fitness(solution);

    if (fitness < best_fitness) {
      best_solution = solution;
      best_fitness = fitness;
    }
  }

  return ResultMH(best_solution, best_fitness, maxevals);
}
