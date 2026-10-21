#pragma once
#include <mddp.h>
#include <mhtrayectory.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class ES : public MHTrayectory {

public:
  ES() : MHTrayectory() {}
  virtual ~ES() {}
  tSolution neighborhood_op(const tSolution solucion_inicial, size_t size, size_t m);
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, const tSolution &current_param,tFitness fitness, int maxevals) override;


};