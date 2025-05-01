#include <cassert>
#include <AGG.h>
#include <iostream>
#include <mddp.h>
#include <util.h>
#include <cassert>

using namespace std;

AGG::AGG(bool c) : MH() {
  this->cruce_uniforme = c;
}

void AGG::reparacion(int n_cruces, vector<int> contador_unos, int m,vector<MddpFactoringInfo> info, vector<tSolution> &soluciones_sel, Mddp *realproblem){
    auto size = realproblem->getProblemSize();
    for(int i=0; i<2*n_cruces; i++){
        if(contador_unos[i]<m){
            while(contador_unos[i]<m){
                float media = info[i].media();
                tOption candidato;
                float dispersion_candidato=-1;
                for(int j=0; j<size; j++){
                    if(soluciones_sel[i][j]==0){
                        float disp = abs(media-realproblem->distanciasIn(soluciones_sel[i], j));
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
                realproblem->updateSolutionFactoringInfoIn(info[i], soluciones_sel[i], candidato);
                soluciones_sel[i][candidato] = 1;
            }
        }else if(contador_unos[i]>m){
            while(contador_unos[i]>m){
                float media = info[i].media();
                tOption candidato;
                float dispersion_candidato=-1;
                for(int j=0; j<size; j++){
                    float disp = abs(media-info[i].info[j]);
                    if(dispersion_candidato==-1 && soluciones_sel[i][j]==1){
                        candidato = j;
                        dispersion_candidato = disp;
                    }else if(soluciones_sel[i][j]==1 && disp>dispersion_candidato){
                        candidato = j;
                        dispersion_candidato = disp;
                    }
                }

                contador_unos[i]--;
                realproblem->updateSolutionFactoringInfoOut(info[i], soluciones_sel[i], candidato);
                soluciones_sel[i][candidato] = 0;                

            }
        }
    }
}

void AGG::cruceUniforme(int n_cruces, vector<tSolution> &soluciones_sel, Mddp *realproblem, int m ){
    vector<MddpFactoringInfo> info;
    vector<int> contador_unos(2*n_cruces, 0);
    auto size = realproblem->getProblemSize();
    for(int i=0; i<2*n_cruces; i+=2){
        for(int j=0; j<size; j++){
            if(soluciones_sel[i][j]!=soluciones_sel[i+1][j]){
                int c = Random::get(0, 1);
                soluciones_sel[i][j]=c;
                if(c==0){
                    c+=1;
                    contador_unos[i+1]++;
                }else{
                    c-=1;
                    contador_unos[i]++;
                }
                soluciones_sel[i+1][j]=c;
            }else if(soluciones_sel[i][j]==1){
                contador_unos[i]++;
                contador_unos[i+1]++;
            }
        }

        info.push_back(realproblem->generateFactoringInfoBinary(soluciones_sel[i]));
        info.push_back(realproblem->generateFactoringInfoBinary(soluciones_sel[i+1]));  

    }

    //Reparacion
    reparacion(n_cruces, contador_unos, m, info, soluciones_sel, realproblem);

}

void AGG::torneo(vector<tSolution> &soluciones_sel, vector<tFitness> &fitness_sel, int size_poblacion, int size, 
                        vector<tSolution> soluciones_inicial, vector<tFitness> fitness_inicial, int m, int n_cruces){
    for(int i=0; i<size_poblacion; i++){
    
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
        }
        
        //Para el cruce necesitamos tener una codificacion binaria de las soluciones
        tSolution new_sol(size, 0);
        for(int j=0; j<m; j++){
            assert(best_index >= 0 && best_index < size_poblacion);
            assert(best_index < soluciones_inicial.size());
            
            auto posi = soluciones_inicial[best_index][j];

            if (!(posi >= 0 && posi < size)) {
                cout <<"Posi: " <<posi <<endl;
                cout <<"Size: " <<size <<endl;
            }
            assert(posi >= 0 && posi < size);
            //new_sol[soluciones_inicial[best_index][j]] = 1;
            new_sol.at(soluciones_inicial[best_index][j]) = 1;
        }
        soluciones_sel.push_back(new_sol);
        fitness_sel.push_back(fitness_inicial[best_index]);
        if(i<2*n_cruces){ //Para recordar que hay que actualizar su fitness
            fitness_sel[i] = -1;
        }

    }
}

void AGG::crucePosicion(int n_cruces, vector<tSolution> &soluciones_sel, size_t size){
    for(int i=0; i<2*n_cruces; i+=2){
            vector<int> aux1;
            vector<int> aux2;
            for(int j=0; j<size; j++){
                if(soluciones_sel[i][j]!=soluciones_sel[i+1][j]){
                    aux1.push_back(soluciones_sel[i][j]);
                    soluciones_sel[i][j] = -1; //Para saber que esa posicion hay que modificarla
                    aux2.push_back(soluciones_sel[i+1][j]);
                }
            }
            Random::shuffle(aux1);
            Random::shuffle(aux2);
            for(int j=0; j<size; j++){
                if(soluciones_sel[i][j]==-1){
                    soluciones_sel[i][j]=aux1.back();
                    aux1.pop_back();
                    soluciones_sel[i+1][j]=aux2.back();
                    aux2.pop_back();
                }
            }
        }
}

void AGG::mutacion(vector<tSolution> &soluciones_sel, size_t size, vector<tFitness> &fitness_sel, int size_poblacion){
    for(int i=0; i<5; i++){
        int index_mut = Random::get(0,size_poblacion-1);
        vector<int> indices_ceros;
        vector<int> indices_unos;

        for (int j = 0; j < size; ++j) {
            if (soluciones_sel[index_mut][j] == 0){
                indices_ceros.push_back(j);
            }else{
                indices_unos.push_back(j);
            }
        }

        int idx_cero = Random::get(0, static_cast<int>(indices_ceros.size()-1));
        int idx_uno  = Random::get(0, static_cast<int>(indices_unos.size()-1));

        swap(soluciones_sel[index_mut][idx_cero], soluciones_sel[index_mut][idx_uno]);
        fitness_sel[index_mut]=-1; //Para luego saber que hay que actualizar el fitness


    }
}

void AGG::reemplazo(vector<tSolution> &soluciones_inicial, vector<tFitness> &fitness_inicial, int size_poblacion, 
                        vector<tSolution> soluciones_sel, vector<tFitness> fitness_sel, tFitness &mejor_fitness, int &mejor_indice, 
                            size_t size, int &evaluaciones, Mddp *realproblem){
    tSolution aux = soluciones_inicial[mejor_indice]; //Lo guardamos por si el mejor de la anterior es mejor
    tFitness nuevo_mejor_fitness = -1;
    int nuevo_mejor_indice;
    for(int i=0; i<size_poblacion; i++){
        tSolution new_sol;
        for(int j=0; j<size;j++){
            if(soluciones_sel[i][j]==1){
                new_sol.push_back(j);
            }
        }
        soluciones_inicial[i] = new_sol;
        fitness_inicial[i] = fitness_sel[i];

        if(fitness_inicial[i]==-1){
            fitness_inicial[i] = realproblem->fitness(soluciones_inicial[i]);
            evaluaciones++;
        }

        if(nuevo_mejor_fitness == -1){
            nuevo_mejor_fitness = fitness_inicial[i];
            nuevo_mejor_indice = i;
        }else if(fitness_inicial[i]<nuevo_mejor_fitness){
            nuevo_mejor_fitness = fitness_inicial[i];
            nuevo_mejor_indice = i;
        }
    }

    if(mejor_fitness<nuevo_mejor_fitness){
        soluciones_inicial[0] = aux;

    }else{
        mejor_fitness=nuevo_mejor_fitness;
        mejor_indice=nuevo_mejor_indice;
    }

}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 * 
 * Esquema de evolución: Basado en un esquema generacional con elitismo
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH AGG::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);
  Mddp *realproblem = dynamic_cast<Mddp *>(problem); 
  int m  = realproblem->getSolutionSize();
  auto size = realproblem->getProblemSize();
  int evaluaciones = 0; 

  //Inicialización de la población inicial
  vector<tSolution> soluciones_inicial;
  vector<tFitness> fitness_inicial;
  int size_poblacion = 50;
  tFitness mejor_fitness = -1;
  int mejor_indice;
  for(int i=0; i<size_poblacion; i++){
    tSolution aux = realproblem->createSolution();
    soluciones_inicial.push_back(aux);
    tFitness fitness = realproblem->fitness(aux);
    fitness_inicial.push_back(fitness);
    if(mejor_fitness == -1){
        mejor_fitness=fitness;
        mejor_indice=i;
    }else if(fitness < mejor_fitness){
        mejor_fitness=fitness;
        mejor_indice=i;
    }
  }

  evaluaciones = size_poblacion;

  while(evaluaciones<maxevals){

    //Población de 50 cromosomas (25 parejas) y una Pcruce de 0.7, cruzarán 0,7*25=17.5 parejas => 18 parejas
    //se cruza el 1 con el 2, el 3 con el 4, el 5 con el 6...
    int n_cruces = 18;

    //Seleccion
    vector<tSolution> soluciones_sel;
    vector<tFitness> fitness_sel;
    torneo(soluciones_sel, fitness_sel, size_poblacion, size,soluciones_inicial,fitness_inicial, m, n_cruces);
    

    //Cruce
    if(cruce_uniforme){
        cruceUniforme(n_cruces, soluciones_sel, realproblem, m );
    }else{ //cruce basado en posicion
        crucePosicion(n_cruces, soluciones_sel,size);
    }

    //Mutacion: Cogemos 5 numero aleatorio de las 50 sol(un 10%), puede haber repeticion, 
    //y de cada uno de esos se coge un elemento de manera aletaoria y se cambia
    mutacion(soluciones_sel, size, fitness_sel, size_poblacion);
    
    
    //Reemplazo
    //La poblacion de los hijos sustituye autimaticamente a la actual, para conservar el elitismo, 
    //si la mejor solucion de la nueva solucion es peor que la mejor solucion dde la anterior poblacion,
    //la mejor solucion de la anterior poblacion sustituye a la mejor solucion de la nueva poblacion
    
    //Retomamos la configuracion inicial de las soluciones y actualizamos los fitness
    reemplazo(soluciones_inicial, fitness_inicial, size_poblacion, soluciones_sel, fitness_sel, mejor_fitness, mejor_indice, size, evaluaciones, realproblem);
    
  }

  return ResultMH(soluciones_inicial[mejor_indice], fitness_inicial[mejor_indice], evaluaciones);
}



//Reparacion
        //Calcular media de distancias
        //Compara con hacerlo a boleo

        // for(int i=0; i<2*n_cruces; i++){
        //     if(contador_unos[i]<m){
        //         while(contador_unos[i]<m){
        //             tFitness mejor_fitness=-1;
        //             int indice;
        //             for(int j=0; j<size; j++){
        //                 if(soluciones_sel[i][j]==0){
        //                     tFitness fit= realproblem->fitnessIn(soluciones_sel[i], info[i], j);
        //                     if(mejor_fitness==-1){
        //                         mejor_fitness = fit;
        //                         indice=j;
        //                     }else if(fit<mejor_fitness){
        //                         mejor_fitness=fit;
        //                         indice=j;
        //                     }
        //                 }
        //             }
        //             contador_unos[i]++;
        //             realproblem->updateSolutionFactoringInfoIn(info[i], soluciones_sel[i], indice);
        //             soluciones_sel[i][indice] = 1;
        //         }
        //     }else if(contador_unos[i]>m){
        //         while(contador_unos[i]>m){
        //             tFitness mejor_fitness=-1;
        //             int indice;
        //             for(int j=0; j<size; j++){
        //                 if(soluciones_sel[i][j]==1){
        //                     tFitness fit= realproblem->fitnessOut(soluciones_sel[i], info[i], j);
        //                     if(mejor_fitness==-1){
        //                         mejor_fitness = fit;
        //                         indice=j;
        //                     }else if(fit<mejor_fitness){
        //                         mejor_fitness=fit;
        //                         indice=j;
        //                     }
        //                 }
        //             }
        //             contador_unos[i]--;
        //             realproblem->updateSolutionFactoringInfoOut(info[i], soluciones_sel[i], indice);
        //             soluciones_sel[i][indice] = 0;
        //         }
        //     }
        // }
