# Complex-Number Scientific Calculator

A collaborative Java desktop calculator with complex-number operations and stack-based interaction.

## Implementation

- [src/main/java](src/main/java/): Swing views, controller and calculator/stack/complex-number models.
- [src/test/java](src/test/java/): JUnit tests for controller, stack, calculator and complex numbers.
- [requirements](requirements/), [deisgn](deisgn/), [project planning](project%20planning/): original requirements, UML, traceability and planning artifacts. The historical `deisgn` spelling is retained to avoid unnecessary document-reference changes.
- [pom.xml](pom.xml): Maven configuration targeting Java 11 and JUnit 5.6.0.

## Validation and running

With Java 11 and Maven available, `mvn test` from this directory is the intended test entry point. This was not executed in the portfolio-refactor environment because Maven and a JDK were unavailable. The POM's `exec.mainClass` names `com.mycompany.prueba.Prueba`, while the archived application entry point is in `scientificcalculatorcomplexnumber`; inspect the IDE/application configuration before launching. No claim is made that all tests currently pass.

## Academic context and authorship

**Team project**, Software Engineering coursework at the University of Salerno, archived within the University of Granada double-degree exchange work. Source author tags include **gabriel**, **fpcolacino** and **jaimecrz3**; these identifiers are preserved rather than expanded into unverified names.
