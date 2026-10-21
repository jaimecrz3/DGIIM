# Metaheuristics

C++ implementations and saved experiments for minimum differential dispersion: select a fixed-size subset while minimizing the range of its elements' pairwise distance sums.

## Implementation and experiments

[Minimum Differential Dispersion](minimum-differential-dispersion/) includes greedy/random baselines, local search, genetic and memetic algorithms, simulated annealing, multistart search, iterated local search and GRASP. Source, benchmark inputs and result tables are retained together.

The current runner lists 50 benchmark instances, defaults to five seeds and passes a 100,000-evaluation budget. Only Random is enabled in its archived configuration; running it does not regenerate every historical table.

## Example from the saved results

For `GKD-b_50_n150_m45`, the trajectory-method table records:

| Method | Fitness (lower is better) | Time (ms) |
|---|---:|---:|
| ILS | 349.4902 | 100 |
| GRASP with local search | 359.3395 | 127 |
| GRASP without local search | 1208.4319 | 1 |

Source: [results3.csv](minimum-differential-dispersion/results3.csv). These rows illustrate the quality/runtime trade-off for one instance and saved experimental setting. They do not establish a general ranking or transferable runtime guarantee. The `.csv` tables are tab-separated; decimal conventions vary between files.

## Further material

- [Implementation, full tables and build notes](minimum-differential-dispersion/)
- [Assignment 1 document](Practica1_MDD.pdf)
- [Assignment 2 document](Pr%C3%A1ctica2_MDD.pdf)
- [Assignment 3 document](Pr%C3%A1ctica3_MDD_.pdf)
- [Randomness and timing examples](tools/)

**Tools:** C++, CMake.

**Academic context:** Metaheuristics, Double Degree in Mathematics and Computer Science, University of Granada. Original source credits and benchmark names are retained.
