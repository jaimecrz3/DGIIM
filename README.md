# Mathematics & Computer Science — University of Granada

Selected implementations, experiments and projects from the five-year **Double Degree in Mathematics and Computer Science** at the **University of Granada, Spain**, including exchange coursework at the University of Salerno.

This archive connects mathematical modelling with software implementation: optimization, machine learning, artificial intelligence, algorithms, numerical image processing, systems and databases. The Intelligent Computing and Systems track and third-year AI submissions are the main portfolio entry points.

## Featured projects

### [](intelligent-computing-and-systems/metaheuristics/minimum-differential-dispersion/)

Select subsets whose pairwise distance sums are balanced. C++ implementations cover local search, genetic and memetic algorithms, simulated annealing, iterated search and GRASP, with benchmark inputs and recorded experiment tables.

**Approach:** shared optimization interfaces and seeded fitness/time comparisons. **Evidence:** four saved result tables; the project README explains the active benchmark configuration and how to avoid overwriting results.

**Topics:** combinatorial optimization · stochastic search · experimental evaluation  
**Tech:** C++ · CMake

### [Machine Learning](intelligent-computing-and-systems/machine-learning/)

Notebook workflows for meat-quality classification and electronic-tongue pH regression, alongside clustering, association rules and neural-network experiments.

**Approach:** exploratory analysis, preprocessing, model selection and validation/test evaluation. **Recorded result:** pH regression with RandomForest reports test **R² ≈ 0.9114** in the supervised-learning notebook. Deep-learning work includes an MNIST convolutional autoencoder.

**Topics:** supervised learning · clustering · representation learning  
**Tech:** Python · scikit-learn · pandas · TensorFlow/Keras

### [Deep Learning for Computer Vision](intelligent-computing-and-systems/computer-vision/deep-learning/)

CNN classification, radiographic age regression and Grad-CAM explanations, documented in three notebooks with saved outputs.

**Approach:** compare scratch training, frozen pretrained features and fine-tuning. **Recorded result:** the ResNet34 fine-tuning evaluation reports test **MAE 5.8631** and **R² 0.8570** for the notebook's age-regression task.

**Topics:** transfer learning · CNNs · visual explanations  
**Tech:** Python · PyTorch/fastai · torchvision

### [Feature Matching and Image Stitching](intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching/)

Harris corner detection, SIFT/texture/deep descriptor matching and homography-based mosaics.

**Approach:** tune detection and suppression parameters, match image features and register images. **Recorded result:** the notebook obtains **503 Harris keypoints** on the assignment image, within its target range of 475–525.

**Topics:** image geometry · feature extraction · registration  
**Tech:** Python · OpenCV · scikit-image · TensorFlow/Keras

### [Parchis Game-Playing Agent](year-3-ugr/artificial-intelligence/parchis-agent/)

Third-year AI integration submission for selecting board-game moves under a search budget.

**Approach:** minimax, alpha-beta pruning, heuristic evaluation and a dynamic-depth variant. The [Rescue Simulation Agents](year-3-ugr/artificial-intelligence/rescue-agent/) implement sensor-driven simulation behaviors. Both require the original course engines; no win-rate or simulation-score claims are made.

**Topics:** adversarial search · decision-making · autonomous agents  
**Tech:** C++

## Coursework

The original academic hierarchy is retained. Only years represented in this archive are listed.

| Academic group | Courses |
|---|---|
| [Second year](year-2/) | [Algorithm design](year-2/algorithm-design/), [data structures](year-2/data-structures/), [object-oriented programming](year-2/object-oriented-programming/) |
| [Third year — Granada](year-3-ugr/) | [Artificial intelligence](year-3-ugr/artificial-intelligence/), [server engineering](year-3-ugr/server-engineering/) |
| [Third year — Salerno](year-3-salerno/) | [Software engineering](year-3-salerno/software-engineering/), [concurrent/parallel programming](year-3-salerno/concurrent-parallel-and-cloud-programming/), [databases](year-3-salerno/databases/), [curves and surfaces](year-3-salerno/curves-and-surfaces/), [differential equations](year-3-salerno/differential-equations/), [probability](year-3-salerno/probability/), [topology](year-3-salerno/topology/) |
| [Fourth year](year-4/) | [Computer graphics](year-4/computer-graphics/) |
| [Intelligent Computing and Systems](intelligent-computing-and-systems/) | [Machine learning](intelligent-computing-and-systems/machine-learning/), [metaheuristics](intelligent-computing-and-systems/metaheuristics/), [computer vision](intelligent-computing-and-systems/computer-vision/), [knowledge engineering](intelligent-computing-and-systems/knowledge-engineering/), [intelligent systems techniques](intelligent-computing-and-systems/intelligent-systems-techniques/), [language processors](intelligent-computing-and-systems/language-processors/), [human-computer interaction](intelligent-computing-and-systems/human-computer-interaction/) |

Other substantial submissions include [Java heuristic-search agents](intelligent-computing-and-systems/intelligent-systems-techniques/heuristic-search/), [MiniZinc optimization models](intelligent-computing-and-systems/intelligent-systems-techniques/constraint-programming/), [PDDL planning](intelligent-computing-and-systems/intelligent-systems-techniques/automated-planning/), a [collaborative CLIPS recipe system](intelligent-computing-and-systems/knowledge-engineering/assignments/submission-2/recipe-recommendation/), a [Unity/Ultraleap planetarium](intelligent-computing-and-systems/human-computer-interaction/gesture-planetarium/) and a [collaborative complex-number calculator](year-3-salerno/software-engineering/scientific-calculator-complex-numbers/).

## Technical areas covered

Algorithms and data structures; artificial intelligence; machine learning and computer vision; combinatorial optimization; constraint programming and automated planning; concurrent/parallel programming; monitoring and systems; databases; computer graphics; geometry, differential equations, probability and topology.

## About this repository

This is historical academic work. Coding conventions, dependencies and APIs reflect the original assignments; a portfolio refresh does not imply a modernization of their algorithms or runtime environments. Reports and notebooks retain their original languages, and internal filenames are preserved where required by code, frameworks, datasets or attribution. English READMEs provide navigation and technical context.

Results quoted above are **saved experiment outputs**, with their scope and prerequisites explained in the linked documentation. There is no global build command. Follow each project's README; some submissions need external datasets, hardware or course frameworks. Collaborative work and third-party materials retain their original credits. Use the archive for learning and review, not for submitting someone else's coursework.
