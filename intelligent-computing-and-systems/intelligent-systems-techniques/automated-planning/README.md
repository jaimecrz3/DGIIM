# Automated Planning Models

PDDL domains and problem instances for a themed resource and movement planning task.

## Objective and approach

Model characters, locations, resources and actions declaratively. Successive `dominio*.pddl` and `problema*.pddl` variants extend the task. [dominio5.pddl](dominio5.pddl) explicitly declares STRIPS, typing, existential preconditions and action costs, including routes with different costs.

## Results and conclusions

The [original report](final-report.pdf) accompanies the models. Standalone generated plans and solver timing logs are not included. The source demonstrates increasingly expressive planning formulations; no optimal-plan or speed claim is made here.

## Running the models

Use a PDDL planner supporting the requirements declared by the selected domain. Pair the matching domain/problem exercise (for example, `dominio5.pddl` and `problema5.pddl`). Planner installation and command-line configuration are not supplied, so a verified universal command is unavailable.

## Academic context

**Course:** Intelligent Systems Techniques. **Technologies:** PDDL. **Author in headers:** Jaime Corzo Galdo. Double Degree in Mathematics and Computer Science, University of Granada.
