#pragma once
#include <mhtrayectory.h>

class BusquedaLocalPM : public MHTrayectory {
public:
  BusquedaLocalPM() : MHTrayectory() {}
  ~BusquedaLocalPM() {}
  ResultMH optimize(Problem *problem, const tSolution &current_param,tFitness fitness, int maxevals) override;
};