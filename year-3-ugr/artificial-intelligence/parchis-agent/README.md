# Parchis Search Agent

C++ game-playing logic exploring adversarial search, move ordering and domain-specific heuristic evaluation in the course Parchis engine.

## Search and evaluation

[AIPlayer.cpp](AIPlayer.cpp) contains minimax, alpha-beta pruning, a dynamic-depth variant and a move-ordered alpha-beta variant. Search uses heuristic evaluation at terminal/depth limits and includes checks against the engine's generated-node limit.

The heuristic variants consider progress toward the goal, pieces at home or at goal, safe positions, captures and board obstacles. The default constructor selects move-ordered alpha-beta with `miValoracion5`, whose scoring also includes endgame terms.

Named configuration values are minimax depth 4, alpha-beta depth 8 and a maximum-depth parameter of 10 for the dynamic variant. They describe the archived implementation, not measured search depth or playing strength.

## Files

- [AIPlayer.cpp](AIPlayer.cpp): search implementations, dispatch and heuristic definitions.
- [AIPlayer.h](AIPlayer.h): player interface and heuristic declarations.

## Archived scope and execution

This is an agent-source submission. The original engine headers, including `../../include/model/Parchis.h`, are absent, so it cannot be built or run independently from this directory. Restore the matching course engine and its expected source layout before attempting execution.

The code includes informal observations about course opponents, but no match logs, win-rate study or timing benchmark accompanies it. Search and heuristic variants are available for inspection; no performance or correctness guarantee is established here.

## Academic context

Artificial Intelligence, Double Degree in Mathematics and Computer Science, University of Granada. Original course interfaces, supplied examples and source credits are retained.
