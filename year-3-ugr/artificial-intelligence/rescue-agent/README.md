# Rescue Simulation Agents

C++ behavior implementations for the course's rescuer and auxiliary agents.

## Objective and approach

Choose actions from sensor observations in a terrain-based rescue simulation. [rescatador.cpp](rescatador.cpp) dispatches behavior by simulation level and contains terrain/height viability checks, orientation-aware neighbor calculations and action selection. The level-3 dispatch is commented out in the archived implementation; this documentation does not imply every level is complete.

## Implementation

- [rescatador.cpp](rescatador.cpp) and [rescatador.hpp](rescatador.hpp): rescuer behavior.
- [auxiliar.cpp](auxiliar.cpp) and [auxiliar.hpp](auxiliar.hpp): auxiliary behavior.

## Results and conclusions

No simulation logs or scores are included. The files document the submitted agent logic, but successful completion of all simulation levels cannot be established from the archive.

## Running the project

Requires the original simulator, including `motorlib/util.h` and the `Comportamientos_Jugador` layout referenced by the source. Those dependencies are not present. Integrate the files into the corresponding course distribution; no standalone executable is supplied.

## Academic context

**Course:** Third-year Artificial Intelligence. **Technologies:** C++. **Degree:** Double Degree in Mathematics and Computer Science, University of Granada. Source interfaces and original credits remain intact.
