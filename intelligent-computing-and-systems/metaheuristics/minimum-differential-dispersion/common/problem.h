#ifndef __PROBLEM_H
#define __PROBLEM_H

#include "solution.h"
#include <random.hpp>
#include <utility>

// get base random alias which is auto seeded and has static API and internal
// state it is not threads secure, you can also use ::random_thread_local
using Random = effolkronium::random_static;

/**
 * Class that represent information useful for factorized solution.
 *
 * The idea is to inherit from this class for specific problems.
 */
class SolutionFactoringInfo {
  public:
  virtual ~SolutionFactoringInfo() = default; // Default destructor

};

/**
 * Problem class to solve the given problem in metaheuristic.
 *
 * @author Jaime Corzo Galdó
 * @version 1.0
 */
class Problem {
public:
  Problem() {}
  virtual ~Problem() {}
  /***
   * Evaluate the solution from scratch.
   * 
   * Para evaluar una solucion dada, siempre ante la misma solucion se debera tener
   * el mismo valor
   * 
   * @param solution to evaluate.
   */
  virtual tFitness fitness(const tSolution &solution) = 0;
  /**
   * Evaluate the solution indicating the current position to change and the new
   * value.
   *
   * By default it run the previous one. However, it can be override to
   * factorize the fitness.
   *
   * @param solution to evaluate.
   * @param solution_info information of solution to calculate the factored
   * fitness.
   * @param pos_change position of the solution to change.
   * @param new_value to store in pos_previous.
   */
  virtual tFitness fitness(const tSolution &solution,
                           SolutionFactoringInfo *solution_info,
                           unsigned pos_change, tDomain new_value) {
    tSolution newsol(solution);
    newsol[pos_change] = new_value;
    return fitness(newsol);
  }

  /**
   * Generate the solution information to generate the factorized fitness.
   * @param solution to generate.
   *
   * @return the solution information.
   */
  virtual SolutionFactoringInfo *
  generateFactoringInfo(const tSolution &solution) {
    return new SolutionFactoringInfo();
  }

  /**
   * Update the Solution Factoring Information after a movement is applied.
   *
   * @param solution.
   * @param solution_info to update, it is updated.
   * @param pos_change position of the solution to change.
   * @param new_value to store in pos_previous.
   */
  virtual void updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                           const tSolution &solution,
                                           unsigned pos_change,
                                           tDomain new_value) {}

  /**
   * Create a new solution de manera aletoria
   */
  virtual tSolution createSolution() = 0;
  /**
   * Return the current size of the solution.
   */
  virtual size_t getSolutionSize() = 0; // Get the size of each solution
  /** Return the range of domain of each element of the solution */
  virtual std::pair<tDomain, tDomain> getSolutionDomainRange() = 0;
};

#endif
