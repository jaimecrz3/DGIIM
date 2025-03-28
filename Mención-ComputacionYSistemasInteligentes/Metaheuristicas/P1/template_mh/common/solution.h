#pragma once
#include <vector>
/**
 * Typedef for the fitness function return type.
 *
 * This is an example, change it for your specific problem.
 */
typedef float tFitness;

/**
 * Domain of each element
 */
typedef int tDomain;

/**
 * Represent a new option to create the solution for the heuristic.
 */
typedef int tOption;

/**
 * Represent the new option to select the heuristic
 */
typedef tFitness tHeuristic;

/**
 * Solution class to represent a solution to the given problem.
 *
 * Tanto el algoritmo como la clase problem deben de trabajar y operar con un tipo 
 * tSolution que representa la solucion del problema.
 */
typedef std::vector<tDomain> tSolution;
