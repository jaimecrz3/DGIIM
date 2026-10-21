#pragma once
#include <AGG.h>
#include <mddp.h>
#include <queue>
#include <BLrandom.h>

using namespace std;

struct ParSolucion {
    tSolution solucion;
    tFitness fitness;
    int index;

    bool operator<(const ParSolucion &other) const {
        return fitness < other.fitness; 
    }
};

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class AM : public AGG {
  //Si vale 1: AM-(10,1.0), si vale 2: AM-(10,0.1) y si vale 3: AM-(10,0.1mej)
  int tipo_hibridacion;
  BusquedaLocalPM BL;

public:
  AM() : AGG() {}
  virtual ~AM() {}
  AM(int type); //Contructor para indicar el tipo de hibridacion
  void AgregarAMejores(priority_queue<ParSolucion> &mejores_5, tSolution sol, tFitness fit, int index);
  void reemplazo(vector<tSolution> &soluciones_inicial, vector<tFitness> &fitness_inicial, int size_poblacion, 
                      vector<tSolution> soluciones_sel, vector<tFitness> fitness_sel, tFitness &mejor_fitness, int &mejor_indice, 
                        size_t size, int &evaluaciones, Mddp *realproblem, priority_queue<ParSolucion> &mejores_5, int m);
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