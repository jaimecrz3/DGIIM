#pragma once
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
class ILS : public MH {
  BusquedaLocalPM BL;

public:
  ILS() : MH() {}
  virtual ~ILS() {}
  void mutacion(tSolution &solucion_inicial, tSolution &no_seleccionados, const int SIZE_MUTATION);
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