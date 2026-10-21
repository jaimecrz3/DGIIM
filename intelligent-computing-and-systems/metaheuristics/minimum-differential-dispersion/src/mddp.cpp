#include <algorithm>
#include <mddp.h>
#include <unordered_set>

float MddpFactoringInfo::media(){
  float media = 0.0;
  int size = info.size();
  int total = 0;
  for(int i=0; i<size; i++){
    if(info[i]!=-1){
      media+=info[i];
      total++;
    }
  }
  return (media/total);

}

Mddp::Mddp(size_t size) : Problem() {
  this->size = size;
}
Mddp::Mddp(const string &file_name) : Problem() {
  ifstream file(file_name);
  if (!file.is_open()) {
    cerr << "Error opening file" << file_name << endl;
    exit(1);
  }
  file >> size;
  file >> sol_size;

   // Inicializamos la matriz con ceros
  matrix.resize(size, vector<float>(size, 0));

  int row, col;
  double value;
  
  while (file >> row >> col >> value) {
      matrix[row][col] = value;
      matrix[col][row] = value; 
  }

  file.close();
}
float Mddp::calcularDelta(const tSolution &solution, int v) {
  double delta = 0.0;
  for (int i = 0; i < solution.size(); i++) {
    delta += matrix[v][solution[i]];
  }
  return delta;
}

tFitness Mddp::fitness(const tSolution &solution) {
   
  tFitness min_fitness = -1;
  tFitness max_fitness = 0;
  for (int i = 0; i < solution.size(); i++) {
    tFitness partial_fitness = 0;
    partial_fitness = calcularDelta(solution, solution[i]);

    if(min_fitness == -1 || partial_fitness < min_fitness){
      min_fitness = partial_fitness;
    }
    if(partial_fitness > max_fitness){
      max_fitness = partial_fitness;
    }

  }
  return max_fitness - min_fitness;

}

tFitness Mddp::fitness(const tSolution &solution,
                           SolutionFactoringInfo *solution_info,
                           unsigned pos_change, tDomain new_value) {

  MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  MddpFactoringInfo info_copia(*info);
  updateSolutionFactoringInfo(&info_copia, solution, pos_change, new_value);
  tFitness max= *max_element(info_copia.info.begin(), info_copia.info.end());
  tFitness min= *min_element(info_copia.info.begin(), info_copia.info.end());
  return max - min;
                        
}

SolutionFactoringInfo *Mddp::generateFactoringInfo(const tSolution &solution){
  MddpFactoringInfo *info = new MddpFactoringInfo();
  for (int i = 0; i < solution.size(); i++) {
      info->info.push_back(calcularDelta(solution, solution[i]));
  }

  return info;
}

void Mddp::updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                    const tSolution &solution,
                                    unsigned pos_change,
                                    tDomain new_value) {
  MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  for(int i=0; i<info->info.size(); i++){
      if(i!=pos_change){
          info->info[i] -= matrix[solution[i]][solution[pos_change]];
          info->info[i] += matrix[solution[i]][new_value];
      }else{
          double delta = 0.0;
          for (int i = 0; i < solution.size(); i++) {
            if(i!=pos_change){
              delta += matrix[new_value][solution[i]];
            }
          }
          info->info[i] = delta;
      }
  }
}
                

tSolution Mddp::createSolution() {

  std::unordered_set<tDomain> solution_set;
    
  while (solution_set.size() < sol_size) {
      tDomain elem = Random::get(0, static_cast<int>(size - 1));
      solution_set.insert(elem);  
  }

  return tSolution(solution_set.begin(), solution_set.end());  

  // tSolution solution(sol_size);
  // vector<int> aux(size);
  // for(int i=0; i<size; i++){
  //   aux[i] = i;
  // }
  // Random::shuffle(aux);       //DUDA->optimizar
  // for (int i = 0; i < solution.size(); i++) {
  //   solution[i] = aux[i];
  // }
  // return solution;
}

tFitness Mddp::fitnessIn(const tSolution &solution,
                           MddpFactoringInfo solution_info,
                           unsigned pos_change){
  //MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  MddpFactoringInfo info_copia(solution_info);
  updateSolutionFactoringInfoIn(info_copia, solution, pos_change);
  tFitness max= *max_element(info_copia.info.begin(), info_copia.info.end());
  tFitness min= *min_element(info_copia.info.begin(), info_copia.info.end());
  //cout << "max: " << max << " min: " << min << endl;
  return max - min;
}

tFitness Mddp::fitnessOut(const tSolution &solution,
                          MddpFactoringInfo solution_info,
                          unsigned pos_change){
  //MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  MddpFactoringInfo info_copia(solution_info);
  updateSolutionFactoringInfoOut(info_copia, solution, pos_change);
  tFitness max= *max_element(info_copia.info.begin(), info_copia.info.end());
  tFitness min= *min_element(info_copia.info.begin(), info_copia.info.end());
  //cout << "max: " << max << " min: " << min << endl;
  return max - min;
}    

// SolutionFactoringInfo *Mddp::generateFactoringInfoBinary(const tSolution &solution_binary){
//   MddpFactoringInfo *info = new MddpFactoringInfo();
//   for (int i = 0; i < solution_binary.size(); i++) {
//     if(solution_binary[i]==1){
//       double delta = 0.0;
//       for (int j = 0; j < solution_binary.size(); j++) {
//         if(solution_binary[j]==1){
//           delta += matrix[i][j];
//         }
//       }
//       info->info.push_back(delta);
//     }else{
//       info->info.push_back(-1.0);
//     }
      
//   }

//   return info;
// }

MddpFactoringInfo Mddp::generateFactoringInfoBinary(const tSolution &solution_binary){
  MddpFactoringInfo info;

  for (int i = 0; i < solution_binary.size(); i++) {
    if (solution_binary[i] == 1) {
      double delta = 0.0;
      for (int j = 0; j < solution_binary.size(); j++) {
        if (solution_binary[j] == 1) {
          delta += matrix[i][j];
        }
      }
      info.info.push_back(delta);
    } else {
      info.info.push_back(-1.0);
    }
  }

  return info;
}


void Mddp::updateSolutionFactoringInfoIn(MddpFactoringInfo &info,
                                  const tSolution &solution,
                                  unsigned pos_change){
 // MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  for(int i=0; i<info.info.size(); i++){
      if(i!=pos_change && info.info[i]!=-1){
          info.info[i] += matrix[i][pos_change];
      }else if(i==pos_change){
          double delta = 0.0;
          for (int j = 0; j < solution.size(); j++) {
            if(j!=pos_change){
              delta += matrix[pos_change][j];
            }
          }
          info.info[i] = delta;
      }
  }
}

float Mddp::distanciasIn(const tSolution &solution,
                                  unsigned pos_change){
 // MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
 float delta = 0.0;
  for(int i=0; i<solution.size(); i++){
      if(solution[i]==1){
          delta += matrix[i][pos_change];
      }
  }
  return delta;
}

void Mddp::updateSolutionFactoringInfoOut(MddpFactoringInfo &info,
                                  const tSolution &solution,
                                  unsigned pos_change){
  //MddpFactoringInfo *info = dynamic_cast<MddpFactoringInfo *>(solution_info);
  for(int i=0; i<info.info.size(); i++){
      if(i!=pos_change && info.info[i]!=-1){
          info.info[i] -= matrix[i][pos_change];
      }else if(i==pos_change){
          info.info[i] = -1.0;
      }
  }
}


