# Parchis Game-Playing Agent

C++ adversarial search for choosing moves in the course Parchis engine.

## Objective and approach

Select a piece and die by searching successor game states. [AIPlayer.cpp](AIPlayer.cpp) contains minimax, alpha-beta pruning and a dynamic-depth variant. Terminal states and depth limits delegate evaluation to a heuristic. The implementation also checks the engine's node budget. Constants set minimax depth to 4, alpha-beta depth to 8 and maximum dynamic depth to 10; these are configuration values, not measured performance.

## Implementation

- [AIPlayer.cpp](AIPlayer.cpp): search and decision logic.
- [AIPlayer.h](AIPlayer.h): player and heuristic declarations.

## Results and conclusions

No tournament logs, win rates or timing measurements accompany this submission. The code supports inspection of the trade-off between deeper search, heuristic evaluation and node limits; it does not establish playing strength empirically.

## Running the project

This is an integration submission, not a standalone executable. It includes `../../include/model/Parchis.h`, which is absent here. Restore these files to the matching course engine's player source layout before compiling with that engine. No independent build command can be verified from this archive.

## Academic context

**Course:** Third-year Artificial Intelligence. **Degree:** Double Degree in Mathematics and Computer Science. **University:** University of Granada. Original course interfaces and source credits are preserved.
