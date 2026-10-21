#pragma once
#include <mh.h>
#include <mddp.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class AGE : public MH {
  //Si es true se hace cruce uniforme y si es false cruce basado en posición
  bool cruce_uniforme;

public:
  AGE() : MH() {}
  virtual ~AGE() {}
  AGE(bool c); //Contructor para indicar el tipo de cruce
  void reparacion(vector<int> contador_unos, int m,vector<MddpFactoringInfo> info, vector<tSolution> &padres, Mddp *realproblem);
  void cruceUniforme(vector<tSolution> &padres, Mddp *realproblem, int m );
  void torneo(vector<tSolution> &padres, int size_poblacion, vector<tFitness> fitness_inicial, vector<tSolution> soluciones_inicial,
                size_t size, int m);
  void crucePosicion( vector<tSolution> &padres, size_t size);
  void mutacion(tSolution &padre, size_t size);
  void reemplazo(vector<tSolution> padres, Mddp *realproblem, size_t size, int &evaluaciones, tFitness &mejor_fitness, int &mejor_indice, 
                    tFitness &peor_fitness, int &index_peor, vector<tFitness> &fitness_inicial, vector<tSolution> &soluciones_inicial);
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