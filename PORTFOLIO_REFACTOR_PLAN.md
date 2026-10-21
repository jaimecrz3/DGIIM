# Portfolio refactor plan

## Audit and scope

Baseline commit: 6fe860c3cbcee5672290f9fdf7e921c23fe4913f. Inventory: 3358 tracked files; 1 pre-existing untracked files. Full tracked tree, blob IDs, initial untracked hashes, and migration map were captured before changes in a temporary audit manifest; the final report records preservation checks.

The archive contains years 2, 3 (Granada and Salerno), year 4 graphics, and the Intelligent Computing and Systems track. No empty year-1/year-5 folders will be invented.

Languages and formats include C/C++, Java, Ruby, Python notebooks, C# (Unity), CLIPS, MiniZinc, PDDL, Lex/Yacc, SQL-related reports, PDF/ODT reports, UML diagrams, images, MATLAB data, CSV/TSV results and spreadsheets. Build systems include CMake, Make, Maven, NetBeans/Ant and Unity. Nine notebooks and a small number of placeholder READMEs are present.

Substantial work: machine-learning and computer-vision notebooks; minimum differential dispersion optimization; CLIPS recipe recommendations; Java heuristic-search agents, MiniZinc constraints and PDDL planning; Unity planetarium; third-year rescue and Parchis agents; Java complex-number calculator; data-structure and algorithm implementations. Mathematics directories mainly contain exercises/reference PDFs and will be described as such.

Reference audit: C++ includes and CLIPS loads use internal Spanish filenames; Java package directories and Unity asset GUID/meta pairs are coupled to their frameworks. Notebooks refer to external/Colab data paths. Generated CMake caches contain historical absolute Linux paths. Preserve internal names rather than risk breaking them. No source identifiers or algorithms will be translated.

## Proposed migration

Paths below are expressed relative to the original root and final root. Directory moves preserve their internal layout unless separately listed. Apply deepest paths first; update text references to full paths and encoded Markdown URLs.

| Current path | Proposed new path | Reason | References to update | Risk |
|---|---|---|---|---|
| `CuartoCurso` | `year-4` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso` | `year-2` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)` | `year-3-salerno` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(UGR)` | `year-3-ugr` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes` | `intelligent-computing-and-systems` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `CuartoCurso/InformaticaGrafica` | `year-4/computer-graphics` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/Algorítmica` | `year-2/algorithm-design` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/EstructurasdeDatos` | `year-2/data-structures` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/PDOO` | `year-2/object-oriented-programming` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/BasidiDati` | `year-3-salerno/databases` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/CurvasySuperfices(GeoIV)` | `year-3-salerno/curves-and-surfaces` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/EquazioniDifferenziali` | `year-3-salerno/differential-equations` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/IngegneriaSoftware` | `year-3-salerno/software-engineering` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/Probabilita` | `year-3-salerno/probability` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/ProgrammazioneConcorrentePallelaeSuCloud` | `year-3-salerno/concurrent-parallel-and-cloud-programming` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(Salerno)/Topologia` | `year-3-salerno/topology` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(UGR)/IA` | `year-3-ugr/artificial-intelligence` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(UGR)/ISE` | `year-3-ugr/server-engineering` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/AprendizajeAutomatico` | `intelligent-computing-and-systems/machine-learning` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/IngenieriaDelConocimiento` | `intelligent-computing-and-systems/knowledge-engineering` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/Metaheuristicas` | `intelligent-computing-and-systems/metaheuristics` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/NuevosParadigmasDeInteracción` | `intelligent-computing-and-systems/human-computer-interaction` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/ProcesadoresDeLenguajes` | `intelligent-computing-and-systems/language-processors` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/TecnicasDeLosSistemasInteligentes` | `intelligent-computing-and-systems/intelligent-systems-techniques` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/VisiónPorComputador` | `intelligent-computing-and-systems/computer-vision` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(UGR)/IA/Practica2` | `year-3-ugr/artificial-intelligence/rescue-agent` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `TercerCurso(UGR)/IA/Practica3` | `year-3-ugr/artificial-intelligence/parchis-agent` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/Metaheuristicas/P1-ProblemaMinimaDispersion` | `intelligent-computing-and-systems/metaheuristics/minimum-differential-dispersion` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/TecnicasDeLosSistemasInteligentes/CorzoGaldoJaime-P1-TSI` | `intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/TecnicasDeLosSistemasInteligentes/CorzoGaldoJaime-P2-TSI` | `intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/TecnicasDeLosSistemasInteligentes/CorzoGaldoJaime-P3-TSI` | `intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/Algorítmica/practica2_dyv` | `year-2/algorithm-design/divide-and-conquer` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/Algorítmica/practica3_greedy` | `year-2/algorithm-design/greedy-algorithms` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `SegundoCurso/Algorítmica/practica4_grafos` | `year-2/algorithm-design/graph-algorithms` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/AprendizajeAutomatico/P1.ipynb` | `intelligent-computing-and-systems/machine-learning/supervised-learning.ipynb` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/AprendizajeAutomatico/P2.ipynb` | `intelligent-computing-and-systems/machine-learning/unsupervised-learning.ipynb` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |
| `Mención-ComputacionYSistemasInteligentes/AprendizajeAutomatico/P3.ipynb` | `intelligent-computing-and-systems/machine-learning/deep-learning.ipynb` | Descriptive English navigation | Search text, Markdown and configuration references; retain internal relative paths | low |

## Deliberate exceptions and cleanup

High-risk or ambiguous internal renames are deferred: Unity Assets/Packages/ProjectSettings and third-party model credits; Java source/package directories; CLIPS modules; build-system filenames; benchmark dataset names; instructor templates and original report/contributor filenames. Document these exceptions in the report. Keep local untracked Planetario_APP files untracked and verify their hashes after moving the enclosing course.

Remove only tracked disposable CMake cache trees, object/dependency files, Python caches and generated CMake metadata identified by path. Retain reports, experiment tables, notebooks and their saved outputs, all datasets, source code, IDE configurations, and uncertain binaries/duplicate submissions. Add conservative ignore rules.

## Documentation and validation

Write English course indexes and detailed project READMEs with evidence links, saved results where interpretable, execution prerequisites and academic attribution. Preserve original contributor information and distinguish design proposals from implemented functionality. Validate local Markdown/image links, notebook JSON, stale full paths, content hashes and available toolchains. Do not run costly training or overwrite results.

## Git workflow

Work locally on portfolio-refresh. Commit organization, course/project documentation, root portfolio README, then reference fixes/cleanup and validation report separately. Do not push or rewrite history.
