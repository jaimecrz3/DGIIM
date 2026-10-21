#pragma once
#include <algorithm>
#include <problem.h>
#include <string>
#include <fstream>
#include <iostream>
#include <random.hpp>

using namespace std;

class MddpFactoringInfo : public SolutionFactoringInfo {

  public: 
    MddpFactoringInfo() {}
    MddpFactoringInfo(const MddpFactoringInfo &factsol) : info(factsol.info) {}
    float media();

    vector<double> info;


};
class Mddp : public Problem {
  size_t size; 
  size_t sol_size;

  vector<vector<float>> matrix;

public:
  Mddp(size_t size);
  Mddp(const string &file_name);

  float calcularDelta(const tSolution &solution, int v);
  tFitness fitness(const tSolution &solution) override;
  tFitness fitness(const tSolution &solution,
                           SolutionFactoringInfo *solution_info,
                           unsigned pos_change, tDomain new_value) override;
  SolutionFactoringInfo *generateFactoringInfo(const tSolution &solution) override;
  void updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                    const tSolution &solution,
                                    unsigned pos_change,
                                    tDomain new_value) override;
  tSolution createSolution() override;
  size_t getProblemSize() { return size; }
  size_t getSolutionSize() override { return sol_size; }
  std::pair<tDomain, tDomain> getSolutionDomainRange() override {
    return std::make_pair(0, size-1);                                        
  }

  tFitness fitnessIn(const tSolution &solution,
                           MddpFactoringInfo solution_info,
                           unsigned pos_change);
  tFitness fitnessOut(const tSolution &solution,
                           MddpFactoringInfo solution_info,
                           unsigned pos_change);                        
  //SolutionFactoringInfo *generateFactoringInfoBinary(const tSolution &solution_binary);
  MddpFactoringInfo generateFactoringInfoBinary(const tSolution &solution_binary);

  void updateSolutionFactoringInfoIn(MddpFactoringInfo &solution_info,
                                    const tSolution &solution,
                                    unsigned pos_change);
  float distanciasIn(const tSolution &solution,
                                  unsigned pos_change);
  void updateSolutionFactoringInfoOut(MddpFactoringInfo &solution_info,
                                    const tSolution &solution,
                                    unsigned pos_change);
};