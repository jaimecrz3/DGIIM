# Heuristic Search Agents

Java agents comparing offline and real-time search in a course game environment.

## Objective and approach

Navigate game states using depth-first search, A*, RTA* and LRTA*-k. Implementations are named `AgenteProfundidad`, `AgenteAStar`, `AgenteRTAStar` and `AgenteLRTAStarK` in the [source package](src_CORZO_GALDO_JAIME/), alongside shared agent/state/node classes.

## Evidence and conclusions

The [original report](final-report.pdf) and [AI interaction disclosure](ai-assistance-disclosure.pdf) are preserved. No standalone machine-readable benchmark output is included; no runtime or optimality comparison is asserted here. This submission exposes different search strategies for code-level comparison.

## Running the project

Requires the course game framework providing `core.game`, `core.player`, `ontology` and related classes, which are absent. Keep the declared package `tracks.singlePlayer.evaluacion.src_CORZO_GALDO_JAIME` when integrating the Java files. The folder name deliberately preserves that package identity.

## Academic context

**Course:** Intelligent Systems Techniques. **Technologies:** Java. **Author identified by submission:** Jaime Corzo Galdo. Double Degree in Mathematics and Computer Science, University of Granada.
