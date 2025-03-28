#include <iostream>
#include <problem.h> 
#include <random.hpp>
#include <string>
#include <util.h>
// Real problem class
#include "mddp.h"

// All all algorithms
#include "BLpm.h"
#include "BLmejor.h"
#include "greedy.h"
#include "randomsearch.h"

using namespace std;
int main(int argc, char *argv[]) {
  long int seed;

  // Seed the random number generator
  if (argc == 1) {
    // Use a fixed seed for reproducibility
    seed = 42;
  } else {
    seed = atoi(argv[1]);
  }

  // Create the algorithms
  RandomSearch ralg = RandomSearch();
  //BruteSearch rbrute = BruteSearch();
  GreedySearch rgreedy = GreedySearch();
  BusquedaLocalMejor rblmejor = BusquedaLocalMejor();
  BusquedaLocalPM rblpm = BusquedaLocalPM();

  // Create the specific problem
  Mddp rproblem = Mddp("data/GKD-b_50_n150_m45.txt");  
  // Solve using evaluations
  vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                            make_pair("Greedy", &rgreedy),
                                            make_pair("BLmejor", &rblmejor),
                                            make_pair("BLprimermejor", &rblpm)};
                                        
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  for (int i = 0; i < algoritmos.size(); i++) {
    Random::seed(seed);
    cout << algoritmos[i].first << endl;
    MH *mh = algoritmos[i].second;
    ResultMH result = mh->optimize(problem, 100000);
    cout << "Best solution: " << result.solution << endl;
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
  }

  return 0;
}
