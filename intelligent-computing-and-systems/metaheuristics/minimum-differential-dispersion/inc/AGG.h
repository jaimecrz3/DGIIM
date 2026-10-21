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
class AGG : public MH {
  //Si es true se hace cruce uniforme y si es false cruce basado en posición
  bool cruce_uniforme;

public:
  AGG() : MH() {}
  virtual ~AGG() {}
  AGG(bool c); //Contructor para indicar el tipo de cruce
  void reparacion(int n_cruces, vector<int> contador_unos, int m,vector<MddpFactoringInfo> info, vector<tSolution> &soluciones_sel, Mddp *realproblem);
  void cruceUniforme(int n_cruces, vector<tSolution> &soluciones_sel, Mddp *realproblem, int m );
  void torneo(vector<tSolution> &soluciones_sel, vector<tFitness> &fitness_sel, int size_poblacion, int size, 
                        vector<tSolution> soluciones_inicial, vector<tFitness> fitness_inicial, int m, int n_cruces);
  void crucePosicion(int n_cruces, vector<tSolution> &soluciones_sel, size_t size);
  void mutacion(vector<tSolution> &soluciones_sel, size_t size, vector<tFitness> &fitness_sel, int size_poblacion);
  void reemplazo(vector<tSolution> &soluciones_inicial, vector<tFitness> &fitness_inicial, int size_poblacion, 
                      vector<tSolution> soluciones_sel, vector<tFitness> fitness_sel, tFitness &mejor_fitness, int &mejor_indice, 
                        size_t size, int &evaluaciones, Mddp *realproblem);
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