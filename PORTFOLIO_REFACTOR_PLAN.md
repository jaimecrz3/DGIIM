# Portfolio refactor plan

## Audit and scope

Baseline commit: 6fe860c3cbcee5672290f9fdf7e921c23fe4913f. Inventory: 3358 tracked files; 1 pre-existing untracked nested repository. Full tracked tree, blob IDs, the untracked repository path, and migration map were captured before changes in a temporary audit manifest; the final report records preservation checks.

The archive contains years 2, 3 (Granada and Salerno), year 4 graphics, and the Intelligent Computing and Systems track. No empty year-1/year-5 folders will be invented.

Languages and formats include C/C++, Java, Ruby, Python notebooks, C# (Unity), CLIPS, MiniZinc, PDDL, Lex/Yacc, SQL-related reports, PDF/ODT reports, UML diagrams, images, MATLAB data, CSV/TSV results and spreadsheets. Build systems include CMake, Make, Maven, NetBeans/Ant and Unity. Eight notebooks and a small number of placeholder READMEs are present.

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

High-risk or ambiguous internal renames are deferred: Unity Assets/Packages/ProjectSettings and third-party model credits; Java source/package directories; CLIPS modules; build-system filenames; benchmark dataset names; instructor templates and original report/contributor filenames. Document these exceptions in the report. Keep local untracked Planetario_APP files untracked and preserve its contents and nested Git repository while moving the enclosing course (a recursive baseline hash was not captured).

Remove only tracked disposable CMake cache trees, object/dependency files, Python caches and generated CMake metadata identified by path. Retain reports, experiment tables, notebooks and their saved outputs, all datasets, source code, IDE configurations, and uncertain binaries/duplicate submissions. Add conservative ignore rules.

## Documentation and validation

Write English course indexes and detailed project READMEs with evidence links, saved results where interpretable, execution prerequisites and academic attribution. Preserve original contributor information and distinguish design proposals from implemented functionality. Validate local Markdown/image links, notebook JSON, stale full paths, content hashes and available toolchains. Do not run costly training or overwrite results.

## Git workflow

Work locally on portfolio-refresh. Commit organization, course/project documentation, root portfolio README, then reference fixes/cleanup and validation report separately. Do not push or rewrite history.

## Additional navigation renames after source inspection

These paths use the intermediate English course roots from the first migration. Internal code/resource names remain untouched. References are updated by resolving each Markdown target before moving its parent.

| Current path | Proposed new path | Reason | References to update | Risk |
|---|---|---|---|---|
| `year-2/data-structures/Ej01_Imagenes-master` | `year-2/data-structures/image-adt` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-2/data-structures/Ej02_Pilas_y_Colas-master` | `year-2/data-structures/stacks-and-queues` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-2/data-structures/Ej03_TDAs_No_Lineales-master` | `year-2/data-structures/non-linear-data-structures` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-2/data-structures/p04_juego_letras-master` | `year-2/data-structures/word-game` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-2/object-oriented-programming/deepspace-master` | `year-2/object-oriented-programming/deepspace` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-4/computer-graphics/practicas` | `year-4/computer-graphics/assignments` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-3-ugr/server-engineering/Bloque1Jaime` | `year-3-ugr/server-engineering/operating-systems-and-services` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-3-ugr/server-engineering/Bloque2Jaime` | `year-3-ugr/server-engineering/monitoring` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `year-3-ugr/server-engineering/Entregas ISE-JCG` | `year-3-ugr/server-engineering/submission-archives` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P1` | `intelligent-computing-and-systems/computer-vision/filtering-and-pyramids` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P2` | `intelligent-computing-and-systems/computer-vision/deep-learning` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P3` | `intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P1/P1_template.ipynb` | `intelligent-computing-and-systems/computer-vision/filtering-and-pyramids/filtering-and-pyramids.ipynb` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P2/P2_Ejs1y2_Template.ipynb` | `intelligent-computing-and-systems/computer-vision/deep-learning/cnn-classification.ipynb` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P2/P2_Ej3_Template.ipynb` | `intelligent-computing-and-systems/computer-vision/deep-learning/transfer-learning.ipynb` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P2/P2_Ej4_Template.ipynb` | `intelligent-computing-and-systems/computer-vision/deep-learning/grad-cam.ipynb` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/computer-vision/P3/P3_template.ipynb` | `intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching/feature-matching-and-stitching.ipynb` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P1` | `intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P2` | `intelligent-computing-and-systems/human-computer-interaction/mobile-companion` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P3` | `intelligent-computing-and-systems/human-computer-interaction/voice-assistant` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas` | `intelligent-computing-and-systems/knowledge-engineering/assignments` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Teoria` | `intelligent-computing-and-systems/knowledge-engineering/written-coursework` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega1` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega2` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega1/p1` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-1` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega1/p2` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-2` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega1/p3` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/recipe-reasoning` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega2/p4` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/ontology` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega2/p5` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recommendation-rules` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/knowledge-engineering/Prácticas/entrega2/p6` | `intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recipe-recommendation` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/language-processors/Practica2` | `intelligent-computing-and-systems/language-processors/assignment-2` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/language-processors/Practica3` | `intelligent-computing-and-systems/language-processors/assignment-3` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/language-processors/Practica4` | `intelligent-computing-and-systems/language-processors/assignment-4` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/language-processors/Practica5` | `intelligent-computing-and-systems/language-processors/assignment-5` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/CorzoGaldoJaime-Memoria-P1-TSI.pdf` | `intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/final-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/CorzoGaldoJaime-PromptsRespuestasIA-P1-TSI.pdf` | `intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/ai-assistance-disclosure.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming/CorzoGaldoJaime-Memoria-P2-TSI.pdf` | `intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming/final-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning/CorzoGaldoJaime-Memoria-P3-TSI.pdf` | `intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning/final-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P2/Memoria Técnica App.pdf` | `intelligent-computing-and-systems/human-computer-interaction/mobile-companion/technical-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P3/CorzoGaldoJaime-MemoriaTecnicaAsistenteVoiceFlow.pdf` | `intelligent-computing-and-systems/human-computer-interaction/voice-assistant/technical-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/P3/CorzoGaldoJaime-EnlaceVideoAsistenteVoiceFlow.docx` | `intelligent-computing-and-systems/human-computer-interaction/voice-assistant/demo-video-link.docx` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |
| `intelligent-computing-and-systems/human-computer-interaction/Memoria Museo.pdf` | `intelligent-computing-and-systems/human-computer-interaction/museum-design-report.pdf` | English assignment/project navigation | Relative Markdown targets; scan source/configuration paths | low |

## Deferred internal names

| Current path or pattern | Proposed handling | Reason | Reference implications | Risk |
|---|---|---|---|---|
| Unity Assets and imported model subdirectories | Retain | Third-party credits, resource names and GUID/meta coupling | Editor/runtime references | high |
| src_CORZO_GALDO_JAIME and Java package directories | Retain | Declared framework package identity | Requires absent course framework | high |
| CLIPS module filenames and load dependencies | Retain | Runtime loading by name; one dependency is missing | Cannot establish complete startup without original dependency | high |
| estudiante, materiales, original algorithm implementation/experiment directories | Retain | Supplied layouts and historical build/script references | Avoid altering instructor materials and uncertain duplicate variants | high |
| Remaining original PDF/ODT titles and author-labelled submissions | Retain | External/reference material or original submission identity | Embedded document links were not exhaustively extracted | high |

Final safe project-root rename: `intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/Planetario_Unity_Leap` to `intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/unity-project`. Reason: English kebab-case project navigation. Update Markdown paths; internal Unity paths/GUIDs remain intact. Risk: low.
