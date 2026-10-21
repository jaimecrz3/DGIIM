
# Minimum Differential Dispersion

C++ benchmark implementations for selecting a fixed-size subset with balanced pairwise distance sums.

## Problem and approach

For each selected element, sum its distances to the other selected elements, then minimize the difference between the largest and smallest sums. This objective is implemented in [src/mddp.cpp](src/mddp.cpp). Lower fitness is better; zero is the lower bound of this range objective.

The [source directory](src/) includes greedy and random search, local search, generational and steady-state genetic algorithms, memetic search, multistart search, simulated annealing, iterated local search and GRASP. These implementations share interfaces in [common](common/) and declarations in [inc](inc/).

## Experimental setup

[main.cpp](main.cpp) enumerates 50 GKD-b instances and defaults to five seeds: 42, 123, 1, 24 and 98. It uses a 100,000-evaluation budget and averages fitness and elapsed milliseconds across seeds. The currently active algorithm is **Random**; the other selections are commented out. Do not assume a default run reproduces every archived table.

## Results and conclusions

Recorded tables: [results.csv](results.csv), [results2.csv](results2.csv), [results3.csv](results3.csv), [results4.csv](results4.csv). Despite the extension, these are tab-separated, with comma decimal separators. The first recorded Random result in `results4.csv`, for `GKD-b_1_n25_m2`, is fitness **0.0000**, time **19 ms**. This reaches the objective's lower bound for that recorded case; it is not evidence of performance on larger instances or other machines. Reports and the [experiment spreadsheet](Tablas_MDD_2024-25.xls) preserve the broader coursework evidence.

## Building and running

Requires a C++ compiler and CMake **3.28.3 or newer**, as declared in [CMakeLists.txt](CMakeLists.txt). From this directory:

```sh
cmake -S . -B build
cmake --build build --config Release
```

The target is `main` (usually `build/main` on single-configuration Unix generators, or `build/Release/main.exe` on Visual Studio). The executable reads `data/` relative to its working directory and **overwrites `results4.csv`**. To protect the archived results, create a separate working directory, copy `data/` into it, and run the built executable from there. Pass either no arguments or exactly five integer seeds. Training/benchmark execution was not performed during the portfolio refactor.

## Academic context

**Course:** Metaheuristics, Intelligent Computing and Systems track. **Degree:** Double Degree in Mathematics and Computer Science, University of Granada. Original source, benchmark names and [license](LICENSE) are retained.
