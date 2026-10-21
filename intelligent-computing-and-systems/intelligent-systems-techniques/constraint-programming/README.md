# Constraint Programming Models

MiniZinc models for discrete feasibility, scheduling and optimization problems.

## Objective and approach

The `.mzn` submissions model a logic puzzle, lexicographic multiobjective knapsack, magic squares, task scheduling, dissertation-defense committees and NBA scheduling. Variables, domains, constraints and solve directives are kept in the original exercise files. Several models use `globals.mzn`.

## Implementation and evidence

- `Ejercicio1a.mzn` through `Ejercicio7.mzn`: exercise models and variants.
- [Original report](final-report.pdf): submitted discussion.

No solver logs are archived separately. Feasibility, optimality and execution times have not been independently re-established during this refactor.

## Running the models

Install MiniZinc and a solver supporting the selected model's constraints. From this directory, a representative invocation is:

```sh
minizinc Ejercicio4.mzn
```

Inspect each model's fixed parameters and `solve` directive before running; solver/runtime requirements vary. The command was not executed in this environment.

## Academic context

**Course:** Intelligent Systems Techniques. **Author in model headers:** Jaime Corzo Galdo. Double Degree in Mathematics and Computer Science, University of Granada.
