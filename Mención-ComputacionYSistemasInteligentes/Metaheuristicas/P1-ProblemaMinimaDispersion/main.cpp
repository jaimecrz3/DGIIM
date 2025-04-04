// #include <iostream>
// #include <problem.h> 
// #include <random.hpp>
// #include <string>
// #include <util.h>
// // Real problem class
// #include "mddp.h"
// #include <chrono>
// using namespace std::chrono;

// // All all algorithms
// #include "BLheur.h"
// #include "BLrandom.h"
// #include "greedy.h"
// #include "randomsearch.h"

// using namespace std;
// int main(int argc, char *argv[]) {
//   vector<long int> seed;

//   // Seed the random number generator
//   if (argc == 1) {
//     // Use a fixed seed for reproducibility
//     seed.push_back(42);
//     seed.push_back(123);
//     seed.push_back(1);
//     seed.push_back(24);
//     seed.push_back(98);
//   } else if(argc == 6){
//     seed.push_back(atoi(argv[1]));
//     seed.push_back(atoi(argv[2]));
//     seed.push_back(atoi(argv[3]));
//     seed.push_back(atoi(argv[4]));
//     seed.push_back(atoi(argv[5]));
//   }

//   // Create the algorithms
//   //RandomSearch ralg = RandomSearch();
//   //BruteSearch rbrute = BruteSearch();
//   GreedySearch rgreedy = GreedySearch();
//   BusquedaLocalMejor rblmejor = BusquedaLocalMejor();
//   BusquedaLocalPM rblpm = BusquedaLocalPM();

//   // Create the specific problem
//   vector<string> files;
//   // files.push_back("GKD-b_1_n25_m2");
//   // files.push_back("GKD-b_2_n25_m2");
//   // files.push_back("GKD-b_3_n25_m2");
//   // files.push_back("GKD-b_4_n25_m2");
//   // files.push_back("GKD-b_5_n25_m2");
//   // files.push_back("GKD-b_6_n25_m7");
//   // files.push_back("GKD-b_7_n25_m7");
//   // files.push_back("GKD-b_8_n25_m7");
//   // files.push_back("GKD-b_9_n25_m7");
//   // files.push_back("GKD-b_10_n25_m7");
//   // files.push_back("GKD-b_11_n50_m5");
//   // files.push_back("GKD-b_12_n50_m5");
//   // files.push_back("GKD-b_13_n50_m5");
//   // files.push_back("GKD-b_14_n50_m5");
//   // files.push_back("GKD-b_15_n50_m5");
//   // files.push_back("GKD-b_16_n50_m15");
//   // files.push_back("GKD-b_17_n50_m15");
//   // files.push_back("GKD-b_18_n50_m15");
//   // files.push_back("GKD-b_19_n50_m15");
//   // files.push_back("GKD-b_20_n50_m15");
//   // files.push_back("GKD-b_21_n100_m10");
//   // files.push_back("GKD-b_22_n100_m10");
//   // files.push_back("GKD-b_23_n100_m10");
//   // files.push_back("GKD-b_24_n100_m10");
//   // files.push_back("GKD-b_25_n100_m10");
//   // files.push_back("GKD-b_26_n100_m30");
//   // files.push_back("GKD-b_27_n100_m30");
//   // files.push_back("GKD-b_28_n100_m30");
//   // files.push_back("GKD-b_29_n100_m30");
//   // files.push_back("GKD-b_30_n100_m30");
//   // files.push_back("GKD-b_31_n125_m12");
//   // files.push_back("GKD-b_32_n125_m12");
//   // files.push_back("GKD-b_33_n125_m12");
//   // files.push_back("GKD-b_34_n125_m12");
//   // files.push_back("GKD-b_35_n125_m12");
//   // files.push_back("GKD-b_36_n125_m37");
//   // files.push_back("GKD-b_37_n125_m37");
//   // files.push_back("GKD-b_38_n125_m37");
//   // files.push_back("GKD-b_39_n125_m37");
//   // files.push_back("GKD-b_40_n125_m37");
//   // files.push_back("GKD-b_41_n150_m15");
//   // files.push_back("GKD-b_42_n150_m15");
//   // files.push_back("GKD-b_43_n150_m15");
//   // files.push_back("GKD-b_44_n150_m15");
//   // files.push_back("GKD-b_45_n150_m15");
//   // files.push_back("GKD-b_46_n150_m45");
//   // files.push_back("GKD-b_47_n150_m45");
//   // files.push_back("GKD-b_48_n150_m45");
//   // files.push_back("GKD-b_49_n150_m45");
//   files.push_back("GKD-b_50_n150_m45");

//   // Solve using evaluations
//   vector<pair<string, MH *>> algoritmos = {make_pair("Greedy", &rgreedy),
//                                             make_pair("BLheur", &rblmejor),
    
//                                           make_pair("BLrandom", &rblpm)};
  
//   for(int i = 0; i < algoritmos.size(); i++){
//     cout << algoritmos[i].first << endl;
//     MH *mh = algoritmos[i].second;
//     for(int f=0; f<files.size(); f++){
//       Mddp rproblem = Mddp("data/"+files[f]+".txt");
//       Problem *problem = dynamic_cast<Problem *>(&rproblem);
//       cout << "File: " << files[f] << endl;

//       milliseconds tiempo_medio(0);
//       tFitness fitness_medio = 0.0;
      
//       for (int s=0; s<seed.size(); s++) {
//         Random::seed(seed[s]);
//         auto momentoInicio = high_resolution_clock::now();
//         ResultMH result = mh->optimize(problem, 100000);
//         auto momentoFin = high_resolution_clock::now();
//         cout << "Best solution: " << result.solution << endl;
//         cout << "Best fitness: " << result.fitness << endl;
//         //cout << "Evaluations: " << result.evaluations << endl;
//         fitness_medio+=result.fitness;
//         // Calculo el tiempo que le ha tomado al algoritmo ejecutarse
//         milliseconds tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
//         tiempo_medio += std::chrono::milliseconds(tiempo.count());
//       }
//       fitness_medio=fitness_medio/5;
//       cout << "Fitness medio: " << fitness_medio << endl;
//       tiempo_medio = std::chrono::milliseconds(tiempo_medio.count() / 5);
//       cout << "Tiempo medio: " << tiempo_medio.count() << " ms" << endl;

//       }
    
//   }

//   return 0;
// }

#include <iostream>
#include <fstream>
#include <problem.h> 
#include <random.hpp>
#include <string>
#include <util.h>
#include "mddp.h"
#include <chrono>
using namespace std::chrono;
using namespace std;

#include "BLrandom.h"
#include "BLheur.h"
#include "greedy.h"
#include "randomsearch.h"

int main(int argc, char *argv[]) {
  vector<long int> seed;
  if (argc == 1) {
    seed = {42, 123, 1, 24, 98};
  } else if (argc == 6) {
    for (int i = 1; i <= 5; i++) {
      seed.push_back(atoi(argv[i]));
    }
  }

  vector<string> files = {
      "GKD-b_1_n25_m2", "GKD-b_2_n25_m2", "GKD-b_3_n25_m2", "GKD-b_4_n25_m2", "GKD-b_5_n25_m2",
      "GKD-b_6_n25_m7", "GKD-b_7_n25_m7", "GKD-b_8_n25_m7", "GKD-b_9_n25_m7", "GKD-b_10_n25_m7",
      "GKD-b_11_n50_m5", "GKD-b_12_n50_m5", "GKD-b_13_n50_m5", "GKD-b_14_n50_m5", "GKD-b_15_n50_m5",
      "GKD-b_16_n50_m15", "GKD-b_17_n50_m15", "GKD-b_18_n50_m15", "GKD-b_19_n50_m15", "GKD-b_20_n50_m15",
      "GKD-b_21_n100_m10", "GKD-b_22_n100_m10", "GKD-b_23_n100_m10", "GKD-b_24_n100_m10", "GKD-b_25_n100_m10",
      "GKD-b_26_n100_m30", "GKD-b_27_n100_m30", "GKD-b_28_n100_m30", "GKD-b_29_n100_m30", "GKD-b_30_n100_m30",
      "GKD-b_31_n125_m12", "GKD-b_32_n125_m12", "GKD-b_33_n125_m12", "GKD-b_34_n125_m12", "GKD-b_35_n125_m12",
      "GKD-b_36_n125_m37", "GKD-b_37_n125_m37", "GKD-b_38_n125_m37", "GKD-b_39_n125_m37", "GKD-b_40_n125_m37",
      "GKD-b_41_n150_m15", "GKD-b_42_n150_m15", "GKD-b_43_n150_m15", "GKD-b_44_n150_m15", "GKD-b_45_n150_m15",
      "GKD-b_46_n150_m45", "GKD-b_47_n150_m45", "GKD-b_48_n150_m45", "GKD-b_49_n150_m45", "GKD-b_50_n150_m45"};

  vector<pair<string, MH *>> algoritmos = {
      {"Greedy", new GreedySearch()},
      {"BLheur", new BusquedaLocalMejor()},
      {"BLrandom", new BusquedaLocalPM()}};

  ofstream xlsFile("results.txt");
  xlsFile << "Algorithm\tFile\tFitness\tTime(ms)" << endl;

  for (auto &algoritmo : algoritmos) {
    MH *mh = algoritmo.second;
    for (const string &file : files) {
      Mddp rproblem("data/" + file + ".txt");
      Problem *problem = dynamic_cast<Problem *>(&rproblem);
      milliseconds tiempo_medio(0);
      tFitness fitness_medio = 0.0;

      for (long int s : seed) {
        Random::seed(s);
        auto start = high_resolution_clock::now();
        ResultMH result = mh->optimize(problem, 100000);
        auto end = high_resolution_clock::now();
        fitness_medio += result.fitness;
        tiempo_medio += duration_cast<milliseconds>(end - start);
      }

      fitness_medio /= seed.size();
      tiempo_medio = milliseconds(tiempo_medio.count() / seed.size());
      xlsFile << algoritmo.first << "\t" << file << "\t" << fitness_medio << "\t" << tiempo_medio.count() << endl;
    }
  }

  xlsFile.close();
  return 0;
}