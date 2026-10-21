#pragma once
#include <mh.h>
#include <mddp.h>
#include <BLrandom.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class GRASP : public MH {
  BusquedaLocalPM BL;
  bool hay_bl;

public:
  GRASP() : MH() {}
  virtual ~GRASP() {}
  GRASP(bool bl); //Contructor para indicar GRASP tanto sin BL como con BL
  tSolution greedy_aleatorizado(Mddp *realproblem, size_t size, size_t m);
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(Problem *problem, int maxevals) override;


};