# Portfolio Refactor Report

## Outcome

Refreshed the academic archive on local branch `portfolio-refresh`, retaining the year/exchange/track hierarchy. Replaced Spanish course abbreviations with English navigation, named major submissions by their technical content, rewrote the root portfolio index and documented courses and projects in English. No algorithms or source identifiers were changed. No push or history rewrite was performed.

Baseline: `6fe860c3cbcee5672290f9fdf7e921c23fe4913f` (3,358 tracked files). The complete [migration manifest](docs/portfolio-migration.tsv) records every original path, final path, original Git blob and disposition. The [plan](PORTFOLIO_REFACTOR_PLAN.md) was created before renaming; later safe naming refinements are recorded there as well.

## Top-level migration

| Original | Final |
|---|---|
| `SegundoCurso/` | [year-2/](year-2/) |
| `TercerCurso(UGR)/` | [year-3-ugr/](year-3-ugr/) |
| `TercerCurso(Salerno)/` | [year-3-salerno/](year-3-salerno/) |
| `CuartoCurso/` | [year-4/](year-4/) |
| `Mención-ComputacionYSistemasInteligentes/` | [intelligent-computing-and-systems/](intelligent-computing-and-systems/) |

Course directories, major assignment wrappers, all eight notebook filenames and selected featured-project reports now have English names. Internal names were retained where coupled to code, supplied materials or provenance. Git moves preserve unchanged file contents and allow normal rename detection; no historical commits were rewritten.

## Documentation and featured work

The archive now has **73 README.md files: 67 added and six rewritten**. The root features minimum differential dispersion optimization, sensor-data machine learning, deep computer vision, feature matching/stitching and the third-year Parchis agent. Supporting project documentation covers rescue behavior, Java heuristic search, MiniZinc models, PDDL planning, recipe recommendations, the Unity planetarium and the complex-number calculator.

Metrics are taken from saved artifacts, not new training runs. Examples: RandomForest pH regression test R² 0.9114068976935454; ResNet34 fine-tuning test MAE 5.8631 and R² 0.8570; 503 Harris keypoints for the documented image and parameters. The transfer-learning outputs were checked against the scratch/frozen/fine-tuning sections (notebook cells 23, 31 and 45, zero-based). The metaheuristics README explains tab-separated/comma-decimal result tables and that the active runner selects Random and overwrites results4.csv.

Authorship is explicit: CLIPS recipe modules credit Jaime Corzo Galdo and Mario Lindez Martinez; calculator source credits gabriel, fpcolacino and jaimecrz3. Unity museum work is described as collaborative without inventing individual contribution boundaries. Existing names, instructor templates and third-party model credits remain. The original Unity README's Spanish design notebook is preserved unchanged as [design-notes.es.md](intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/unity-project/design-notes.es.md); its proposals are distinguished from implemented features.

## Cleanup and preservation

Removed **305** disposable tracked CMake/compiler files: cache trees, compiler-identification probes, generated CMake makefiles/metadata, object files and dependency files. Cleanup was restricted to explicit paths inside this workspace. Build artifacts requiring forced index removal were first checked against baseline Git blobs.

Retained and verified **3,047** original non-README files against baseline content, accepting exact raw bytes or Git checkout line-ending normalization. Eight original files require the raw-byte comparison because their baseline blobs contain CRLF; they were not changed. The remaining six baseline files are deliberately rewritten READMEs. The manifest therefore accounts for all 3,358 original files: 3,047 preserved + 305 disposable artifacts removed + six documentation rewrites. Notebook code, metadata and saved outputs, PDFs, datasets, experimental CSV/TSV tables, spreadsheets and substantive code are preserved.

Conservative ignore rules cover compiler/Python/CMake/Unity caches and the existing local Android repository. Uncertain executables, IDE configuration, alternative submissions and original archives were retained. The initial blob inventory contained **138 groups of identical blobs**; these are not automatically obsolete (many are repeated metadata/assets or submission variants), so no general deduplication was attempted.

The pre-existing, untracked `Planetario_APP` nested repository moved with its enclosing course to `intelligent-computing-and-systems/human-computer-interaction/mobile-companion/Planetario_APP/`. Its source was not edited or added to the parent repository. Its Git HEAD remains readable. The initial audit recorded the directory as one untracked repository rather than recursively hashing its contents; preservation of every pre-existing local change cannot therefore be independently certified. Its current nested Git status reports missing IDE files; without an initial nested status, these cannot be attributed to this refactor and were left untouched. No local Android implementation claims were used to describe the published archive.

## Validation performed

- Ran the [static validator](tools/validate-portfolio.ps1): local Markdown links/images exist with exact case; all eight notebooks parse as nbformat 4; original substantive contents match the manifest; removed build artifacts are absent.
- Searched the archive, including notebooks and hidden configuration outside Git/local Android data, for old top-level roots and renamed implementation-wrapper paths. No active references remained outside intentional plan/report/manifest history.
- Checked original Unity design-note preservation against its original Git blob.
- Inspected build entry points and dependency declarations: CMake/Make, Java 11/Maven/JUnit, NetBeans/Ant, Unity/Ultraleap, CLIPS, MiniZinc and PDDL framework requirements.
- Ran Git whitespace checks and reviewed staged changes, commit history and final status. Source changes were limited to moves; runtime imports/includes did not need editing.

Re-run the static checks from the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/validate-portfolio.ps1
```

Final staged validation: **76 Markdown documents, 361 local link/image targets, eight valid notebooks, and 3,047 preserved original files**. All static checks passed.

## Execution limitations and review items

CMake, C/C++ compilers, Maven, a JDK, Make, MPI, MiniZinc and CLIPS were unavailable on PATH. Therefore no compilation, JUnit/MPI/solver run or project test pass is claimed. A .NET installation alone cannot compile the Unity project without its editor and libraries. No notebook training, downloaded dataset, external URL check or hardware interaction was run. The static validator checks local target paths, not Markdown fragment anchors; embedded notebook figures remain in their unchanged notebooks. PDF/ODT embedded links were not exhaustively extracted or rewritten.

Third-year AI needs absent course engines (`Parchis.h`, simulator behavior headers and motorlib); Java search needs its course game framework. The CLIPS recipe startup references missing `SE_clasificar_alimentos.clp`. The calculator POM names a historical main class that differs from the archived application entry point. These pre-existing runtime limitations are documented rather than repaired by guessing. Exact numerical conclusions from unextracted PDF reports were not invented.

Review these deliberate naming exceptions if you later restore the original runtime environments: Unity Assets/Packages/ProjectSettings and imported names; Java package directories; CLIPS modules and benchmark filenames; supplied `estudiante`/`materiales` layouts; remaining historical implementation/experiment directories and source filenames; original reference/report titles with contributor/submission identities. Their renaming could affect embedded references, scripts or attribution, so they remain unchanged. The original calculator `deisgn` folder is also retained. The English course/project indexes provide descriptive navigation over these preserved internals.

Standalone-project migration to other repositories was not established from the files inspected, so the root README does not claim that any project has moved elsewhere. No new licensing, dependency modernization or algorithm correctness claim is implied by this documentation refresh.

## READMEs added
- [docs/README.md](docs/README.md)
- [intelligent-computing-and-systems/computer-vision/deep-learning/README.md](intelligent-computing-and-systems/computer-vision/deep-learning/README.md)
- [intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching/README.md](intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching/README.md)
- [intelligent-computing-and-systems/computer-vision/filtering-and-pyramids/README.md](intelligent-computing-and-systems/computer-vision/filtering-and-pyramids/README.md)
- [intelligent-computing-and-systems/computer-vision/README.md](intelligent-computing-and-systems/computer-vision/README.md)
- [intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/README.md](intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/README.md)
- [intelligent-computing-and-systems/human-computer-interaction/mobile-companion/README.md](intelligent-computing-and-systems/human-computer-interaction/mobile-companion/README.md)
- [intelligent-computing-and-systems/human-computer-interaction/README.md](intelligent-computing-and-systems/human-computer-interaction/README.md)
- [intelligent-computing-and-systems/human-computer-interaction/voice-assistant/README.md](intelligent-computing-and-systems/human-computer-interaction/voice-assistant/README.md)
- [intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning/README.md](intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning/README.md)
- [intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming/README.md](intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming/README.md)
- [intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/README.md](intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/README.md)
- [intelligent-computing-and-systems/intelligent-systems-techniques/README.md](intelligent-computing-and-systems/intelligent-systems-techniques/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-1/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-1/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-2/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/assignment-2/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/recipe-reasoning/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-1/recipe-reasoning/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/ontology/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/ontology/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recipe-recommendation/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recipe-recommendation/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recommendation-rules/README.md](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recommendation-rules/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/README.md](intelligent-computing-and-systems/knowledge-engineering/README.md)
- [intelligent-computing-and-systems/knowledge-engineering/written-coursework/README.md](intelligent-computing-and-systems/knowledge-engineering/written-coursework/README.md)
- [intelligent-computing-and-systems/language-processors/assignment-2/README.md](intelligent-computing-and-systems/language-processors/assignment-2/README.md)
- [intelligent-computing-and-systems/language-processors/assignment-3/README.md](intelligent-computing-and-systems/language-processors/assignment-3/README.md)
- [intelligent-computing-and-systems/language-processors/assignment-4/README.md](intelligent-computing-and-systems/language-processors/assignment-4/README.md)
- [intelligent-computing-and-systems/language-processors/assignment-5/README.md](intelligent-computing-and-systems/language-processors/assignment-5/README.md)
- [intelligent-computing-and-systems/language-processors/README.md](intelligent-computing-and-systems/language-processors/README.md)
- [intelligent-computing-and-systems/machine-learning/README.md](intelligent-computing-and-systems/machine-learning/README.md)
- [intelligent-computing-and-systems/metaheuristics/README.md](intelligent-computing-and-systems/metaheuristics/README.md)
- [intelligent-computing-and-systems/metaheuristics/tools/README.md](intelligent-computing-and-systems/metaheuristics/tools/README.md)
- [intelligent-computing-and-systems/README.md](intelligent-computing-and-systems/README.md)
- [tools/README.md](tools/README.md)
- [year-2/algorithm-design/README.md](year-2/algorithm-design/README.md)
- [year-2/data-structures/image-adt/README.md](year-2/data-structures/image-adt/README.md)
- [year-2/data-structures/non-linear-data-structures/README.md](year-2/data-structures/non-linear-data-structures/README.md)
- [year-2/data-structures/README.md](year-2/data-structures/README.md)
- [year-2/data-structures/stacks-and-queues/README.md](year-2/data-structures/stacks-and-queues/README.md)
- [year-2/data-structures/word-game/README.md](year-2/data-structures/word-game/README.md)
- [year-2/object-oriented-programming/deepspace/README.md](year-2/object-oriented-programming/deepspace/README.md)
- [year-2/object-oriented-programming/README.md](year-2/object-oriented-programming/README.md)
- [year-2/README.md](year-2/README.md)
- [year-3-salerno/concurrent-parallel-and-cloud-programming/lab4/README.md](year-3-salerno/concurrent-parallel-and-cloud-programming/lab4/README.md)
- [year-3-salerno/concurrent-parallel-and-cloud-programming/lab5/README.md](year-3-salerno/concurrent-parallel-and-cloud-programming/lab5/README.md)
- [year-3-salerno/concurrent-parallel-and-cloud-programming/lab6/README.md](year-3-salerno/concurrent-parallel-and-cloud-programming/lab6/README.md)
- [year-3-salerno/concurrent-parallel-and-cloud-programming/lab8/README.md](year-3-salerno/concurrent-parallel-and-cloud-programming/lab8/README.md)
- [year-3-salerno/concurrent-parallel-and-cloud-programming/README.md](year-3-salerno/concurrent-parallel-and-cloud-programming/README.md)
- [year-3-salerno/curves-and-surfaces/README.md](year-3-salerno/curves-and-surfaces/README.md)
- [year-3-salerno/databases/README.md](year-3-salerno/databases/README.md)
- [year-3-salerno/differential-equations/README.md](year-3-salerno/differential-equations/README.md)
- [year-3-salerno/probability/README.md](year-3-salerno/probability/README.md)
- [year-3-salerno/README.md](year-3-salerno/README.md)
- [year-3-salerno/software-engineering/README.md](year-3-salerno/software-engineering/README.md)
- [year-3-salerno/software-engineering/scientific-calculator-complex-numbers/README.md](year-3-salerno/software-engineering/scientific-calculator-complex-numbers/README.md)
- [year-3-salerno/topology/README.md](year-3-salerno/topology/README.md)
- [year-3-ugr/artificial-intelligence/parchis-agent/README.md](year-3-ugr/artificial-intelligence/parchis-agent/README.md)
- [year-3-ugr/artificial-intelligence/README.md](year-3-ugr/artificial-intelligence/README.md)
- [year-3-ugr/artificial-intelligence/rescue-agent/README.md](year-3-ugr/artificial-intelligence/rescue-agent/README.md)
- [year-3-ugr/README.md](year-3-ugr/README.md)
- [year-3-ugr/server-engineering/monitoring/README.md](year-3-ugr/server-engineering/monitoring/README.md)
- [year-3-ugr/server-engineering/operating-systems-and-services/README.md](year-3-ugr/server-engineering/operating-systems-and-services/README.md)
- [year-3-ugr/server-engineering/README.md](year-3-ugr/server-engineering/README.md)
- [year-3-ugr/server-engineering/submission-archives/README.md](year-3-ugr/server-engineering/submission-archives/README.md)
- [year-4/computer-graphics/assignments/README.md](year-4/computer-graphics/assignments/README.md)
- [year-4/computer-graphics/README.md](year-4/computer-graphics/README.md)
- [year-4/README.md](year-4/README.md)
