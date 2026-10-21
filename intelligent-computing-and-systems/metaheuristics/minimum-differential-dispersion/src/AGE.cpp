#include <cassert>
#include <AGE.h>
#include <iostream>
#include <mddp.h>
#include <util.h>

using namespace std;

AGE::AGE(bool c) : MH() {
  this->cruce_uniforme = c;
}

void AGE::torneo(vector<tSolution> &padres, int size_poblacion, vector<tFitness> fitness_inicial, vector<tSolution> soluciones_inicial, size_t size, int m){
    for(int i=0; i<2; i++){
    
        //Torneo: Cogemos tres de manera aleatoria(se puede repetir) y nos quedamos con el mejor
        int index1 = Random::get(0, static_cast<int>(size_poblacion - 1));
        int index2 = Random::get(0, static_cast<int>(size_poblacion - 1));
        int index3 = Random::get(0, static_cast<int>(size_poblacion - 1));
        int best_index = index1;
        tFitness fitness1 = fitness_inicial[index1];
        tFitness fitness2 = fitness_inicial[index2];
        tFitness fitness3 = fitness_inicial[index3];
        tFitness best_fitness = fitness1;
        if(fitness2 < best_fitness){
            best_index = index2;
            best_fitness = fitness2;
        }
        if(fitness3 < best_fitness){
            best_index = index3;
            best_fitness = fitness3;
        }

        
        //Para el cruce necesitamos tener una codificacion binaria de las soluciones
        tSolution new_sol(size, 0);
        for(int j=0; j<m; j++){
            new_sol[soluciones_inicial[best_index][j]] = 1;
        }
        
        padres.push_back(new_sol);
    }
}

void AGE::reparacion( vector<int> contador_unos, int m,vector<MddpFactoringInfo> info, vector<tSolution> &padres, Mddp *realproblem){
    auto size = realproblem->getProblemSize();
    int n_cruces = 1;
    for(int i=0; i<2*n_cruces; i++){
        if(contador_unos[i]<m){
            while(contador_unos[i]<m){
                float media = info[i].media();
                tOption candidato;
                float dispersion_candidato=-1;
                for(int j=0; j<size; j++){
                    if(padres[i][j]==0){
                        float disp = abs(media-realproblem->distanciasIn(padres[i], j));
                        if(dispersion_candidato==-1){
                            candidato = j;
                            dispersion_candidato = disp;
                        }else if(disp<dispersion_candidato){
                            candidato = j;
                            dispersion_candidato = disp;
                        }
                    }
                }

                contador_unos[i]++;
                realproblem->updateSolutionFactoringInfoIn(info[i], padres[i], candidato);
                padres[i][candidato] = 1;
            }
        }else if(contador_unos[i]>m){
            while(contador_unos[i]>m){
                float media = info[i].media();
                tOption candidato;
                float dispersion_candidato=-1;
                for(int j=0; j<size; j++){
                    float disp = abs(media-info[i].info[j]);
                    if(dispersion_candidato==-1 && padres[i][j]==1){
                        candidato = j;
                        dispersion_candidato = disp;
                    }else if(padres[i][j]==1 && disp>dispersion_candidato){
                        candidato = j;
                        dispersion_candidato = disp;
                    }
                }

                contador_unos[i]--;
                realproblem->updateSolutionFactoringInfoOut(info[i],padres[i], candidato);
                padres[i][candidato] = 0;
            }
        }
    }
}

void AGE::cruceUniforme(vector<tSolution> &padres, Mddp *realproblem, int m ){
    vector<MddpFactoringInfo> info;
    int n_cruces = 1;
    vector<int> contador_unos(2*n_cruces, 0);
    auto size = realproblem->getProblemSize();
    for(int i=0; i<2*n_cruces; i+=2){
        for(int j=0; j<size; j++){
            if(padres[i][j]!=padres[i+1][j]){
                int c = Random::get(0, 1);
                padres[i][j]=c;
                if(c==0){
                    c+=1;
                    contador_unos[1]++;
                }else{
                    c-=1;
                    contador_unos[0]++;
                }
                padres[i+1][j]=c;
            }else if(padres[i][j]==1){
                contador_unos[0]++;
                contador_unos[1]++;
            }
        }

        info.push_back(realproblem->generateFactoringInfoBinary(padres[i]));
        info.push_back(realproblem->generateFactoringInfoBinary(padres[i+1]));
    }
    //Reparacion
    reparacion( contador_unos, m, info, padres, realproblem);
    
}

void AGE::crucePosicion(vector<tSolution> &padres, size_t size){
    for(int i=0; i<2; i+=2){
        vector<int> aux1;
        vector<int> aux2;
        for(int j=0; j<size; j++){
            if(padres[i][j]!=padres[i+1][j]){
                aux1.push_back(padres[i][j]);
                padres[i][j] = -1; //Para saber que esa posicion hay que modificarla
                aux2.push_back(padres[i+1][j]);
            }
        }
        Random::shuffle(aux1);
        Random::shuffle(aux2);
        for(int j=0; j<size; j++){
            if(padres[i][j]==-1){
                padres[i][j]=aux1.back();
                aux1.pop_back();
                padres[i+1][j]=aux2.back();
                aux2.pop_back();
            }
        }
    }
}

void AGE::mutacion(tSolution &padre, size_t size){
    int mut = Random::get(1,100);
    if(mut<=10){
        vector<int> indices_ceros;
        vector<int> indices_unos;

        for (int j = 0; j < size; ++j) {
            if (padre[j] == 0){
                indices_ceros.push_back(j);
            }else{
                indices_unos.push_back(j);
            }
        }

        int idx_cero = Random::get(0, static_cast<int>(indices_ceros.size()-1));
        int idx_uno  = Random::get(0, static_cast<int>(indices_unos.size()-1));

        swap(padre[idx_cero], padre[idx_uno]);
    }
}

void AGE::reemplazo(vector<tSolution> padres, Mddp *realproblem, size_t size, int &evaluaciones, tFitness &mejor_fitness, int &mejor_indice, 
                    tFitness &peor_fitness, int &index_peor, vector<tFitness> &fitness_inicial, vector<tSolution> &soluciones_inicial){
    for(int i=0; i<2; i++){
        tSolution new_sol;
        for(int j=0; j<size;j++){
            if(padres[i][j]==1){
                new_sol.push_back(j);
            }
        }
        padres[i] = new_sol;
    }
    tFitness fitness1 = realproblem->fitness(padres[0]);
    tFitness fitness2 = realproblem->fitness(padres[1]);
    evaluaciones+=2;
    int index_reemplazamiento = 0;
    tFitness reemplazamiento = fitness1;
    if(fitness2<reemplazamiento){
      reemplazamiento=fitness2;
      index_reemplazamiento=1;
    }

    if(reemplazamiento<mejor_fitness){
        mejor_fitness = reemplazamiento;
        mejor_indice = index_reemplazamiento;
    }

    if(reemplazamiento<peor_fitness){
        soluciones_inicial[index_peor] = padres[index_reemplazamiento];
        fitness_inicial[index_peor] = reemplazamiento;

        //Acualizamos el peor fitness
        bool primer=true;
        for(int i=0; i<fitness_inicial.size(); i++){
            if(primer){
                index_peor = i;
                peor_fitness = fitness_inicial[i];
                primer=false;
            }else if(fitness_inicial[i]>peor_fitness){
                index_peor = i;
                peor_fitness = fitness_inicial[i];
            }
        }
    }
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 *  Esquema de evolución: Basado en un esquema estacionario
 * 
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH AGE::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  Mddp *realproblem = dynamic_cast<Mddp *>(problem); 
  int m  = realproblem->getSolutionSize();
  auto size = realproblem->getProblemSize();

  int evaluaciones = 0; //DONDE SE SUMAN LAS EVALUACIONES?

  //Inicialización de la población inicial
  vector<tSolution> soluciones_inicial;
  vector<tFitness> fitness_inicial;
  int size_poblacion = 50;
  tFitness mejor_fitness = -1;
  int mejor_indice;
  tFitness peor_fitness;
  int index_peor;
  for(int i=0; i<size_poblacion; i++){
    tSolution aux = realproblem->createSolution();
    soluciones_inicial.push_back(aux);
    tFitness fitness = realproblem->fitness(aux);
    fitness_inicial.push_back(fitness);
    if(mejor_fitness == -1){
        mejor_fitness=fitness;
        mejor_indice=i;
        index_peor = i;
        peor_fitness = fitness;
    }else if(fitness < mejor_fitness){
        mejor_fitness=fitness;
        mejor_indice=i;
    }else if(fitness>peor_fitness){
        index_peor = i;
        peor_fitness = fitness;
    }
  }

  evaluaciones = size_poblacion;

  while(evaluaciones<maxevals){


    //Seleccion
    //¿El resultado del torneo se sustituye directamente o se crea una copia de poblacion con los ganadores?
    vector<tSolution> padres;
    torneo(padres, size_poblacion, fitness_inicial, soluciones_inicial, size, m);

    //Cruce

    if(cruce_uniforme){
        cruceUniforme(padres, realproblem, m);
    }else{ //cruce basado en posicion
        crucePosicion(padres,size);
    }

    //Mutacion: Cada uno de los padres tiene una probabilidad de 0,1 de mutar
    mutacion(padres[0], size);
    mutacion(padres[1], size);

    //Reemplazo
    //Los dos descendientes generados tras el cruce y la mutacion compiten entre si,
    //y el mejor de ellos sustituye a la peor solucion de la poblacion actual, en caso de ser mejor que ella
    
    //Retomamos la configuracion inicial de las soluciones y actualizamos los fitness
    reemplazo(padres, realproblem, size, evaluaciones, mejor_fitness, mejor_indice, peor_fitness, index_peor, fitness_inicial,soluciones_inicial);

  }


  return ResultMH(soluciones_inicial[mejor_indice], mejor_fitness, evaluaciones);
}