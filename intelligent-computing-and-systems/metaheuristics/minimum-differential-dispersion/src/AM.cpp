#include <cassert>
#include <AM.h>
#include <iostream>
#include <mddp.h>
#include <util.h>
#include <unordered_set>
#include <queue>

using namespace std;

AM::AM(int type) : AGG() {
  this->tipo_hibridacion = type;
}

void AM::AgregarAMejores(priority_queue<ParSolucion> &mejores_5, tSolution sol, tFitness fit, int index) {
    if (mejores_5.size() < 5) {
        mejores_5.push({sol, fit, index});
    } else if (fit < mejores_5.top().fitness) {
        mejores_5.pop();
        mejores_5.push({sol, fit, index});
    }
}


void AM::reemplazo(vector<tSolution> &soluciones_inicial, vector<tFitness> &fitness_inicial, int size_poblacion, 
                        vector<tSolution> soluciones_sel, vector<tFitness> fitness_sel, tFitness &mejor_fitness, int &mejor_indice, 
                            size_t size, int &evaluaciones, Mddp *realproblem, priority_queue<ParSolucion> &mejores_5, int m){
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

        if(tipo_hibridacion==3){
            AgregarAMejores(mejores_5, new_sol, fitness_inicial[i], i);
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
ResultMH AM::optimize(Problem *problem, int maxevals){

  assert(maxevals > 0);
  Mddp *realproblem = dynamic_cast<Mddp *>(problem); 
  int m  = realproblem->getSolutionSize();
  auto size = realproblem->getProblemSize();
  int evaluaciones = 0; 
  const int MAX_EVALS_BL=400;
  unordered_set<int> indices;
  priority_queue<ParSolucion> mejores_5;

  //Inicialización de la población inicial
  vector<tSolution> soluciones_inicial;
  vector<tFitness> fitness_inicial;
  vector<tSolution> no_seleccionados;
  int size_poblacion = 50;
  tFitness mejor_fitness = -1;
  int mejor_indice;
  for(int i=0; i<size_poblacion; i++){
    tSolution aux = realproblem->createSolution();
    soluciones_inicial.push_back(aux);
    tFitness fitness = realproblem->fitness(aux);
    fitness_inicial.push_back(fitness);

    if(tipo_hibridacion==3){
        AgregarAMejores(mejores_5, aux, fitness, i);
    }

    if(mejor_fitness == -1){
        mejor_fitness=fitness;
        mejor_indice=i;
    }else if(fitness < mejor_fitness){
        mejor_fitness=fitness;
        mejor_indice=i;
    }
  }

  evaluaciones = size_poblacion;
  //Contador de generaciones para cuando llegue a la 10 hacer la hibridacion
  int n_generaciones = 0;
  

  while(evaluaciones<maxevals){

    if(n_generaciones==10){
        priority_queue<ParSolucion> cola(mejores_5);
        n_generaciones=0;
        switch (tipo_hibridacion)
        {
        case 1:
            for(int i=0; i<size_poblacion; i++){
                ResultMH result = BL.optimize(realproblem, soluciones_inicial[i], fitness_inicial[i], MAX_EVALS_BL);
                evaluaciones += result.evaluations;
                fitness_inicial[i]=result.fitness;
                soluciones_inicial[i] = result.solution;
            }
            break;
        case 2:
            while (indices.size() < 5) {
                indices.insert(Random::get(0, 49));
            }
            for(int valor : indices){
                ResultMH result = BL.optimize( realproblem, soluciones_inicial[valor], fitness_inicial[valor], MAX_EVALS_BL);
                evaluaciones += result.evaluations;
                fitness_inicial[valor]=result.fitness;
                soluciones_inicial[valor] = result.solution;
            }
            break;
        case 3:
            while (!cola.empty()) {
                int a = cola.top().index;
                ResultMH result = BL.optimize(realproblem, cola.top().solucion, cola.top().fitness, MAX_EVALS_BL);
                evaluaciones += result.evaluations;
                fitness_inicial[cola.top().index]=result.fitness;
                soluciones_inicial[cola.top().index] = result.solution;
                cola.pop();
            }
            break;

        default:
            break;
        }
    }

    //Población de 50 cromosomas (25 parejas) y una Pcruce de 0.7, cruzarán 0,7*25=17.5 parejas => 18 parejas
    //se cruza el 1 con el 2, el 3 con el 4, el 5 con el 6...
    int n_cruces = 18;

    //Seleccion
    vector<tSolution> soluciones_sel;
    vector<tFitness> fitness_sel;
    soluciones_sel.reserve(size_poblacion);
    fitness_sel.reserve(size_poblacion);
    torneo(soluciones_sel, fitness_sel, size_poblacion, size,soluciones_inicial,fitness_inicial, m, n_cruces);
    

    //Cruce
    cruceUniforme(n_cruces, soluciones_sel, realproblem, m );
    
    //Mutacion: Cogemos 5 numero aleatorio de las 50 sol(un 10%), puede haber repeticion, 
    //y de cada uno de esos se coge un elemento de manera aletaoria y se cambia
    mutacion(soluciones_sel, size, fitness_sel, size_poblacion);
    
    
    //Reemplazo
    //La poblacion de los hijos sustituye autimaticamente a la actual, para conservar el elitismo, 
    //si la mejor solucion de la nueva solucion es peor que la mejor solucion dde la anterior poblacion,
    //la mejor solucion de la anterior poblacion sustituye a la mejor solucion de la nueva poblacion
    
    //Retomamos la configuracion inicial de las soluciones y actualizamos los fitness
    reemplazo(soluciones_inicial, fitness_inicial, size_poblacion, soluciones_sel, fitness_sel, mejor_fitness, mejor_indice, size, evaluaciones, realproblem, mejores_5, m);
    
        
    n_generaciones++;
  }

  return ResultMH(soluciones_inicial[mejor_indice], fitness_inicial[mejor_indice], evaluaciones);


}