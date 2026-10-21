# Mathematics & Computer Science — University of Granada

Selected projects and implementations from the five-year Double Degree in Mathematics and Computer Science at the University of Granada, including exchange coursework at the University of Salerno.

This archive documents foundations in algorithm design, optimization, search, statistical learning and software development that underpin my current work in quantitative finance, mathematical modelling, optimization, machine learning and quantitative development. The main implementations use C++ and Python.

## Featured Projects

### [Minimum Differential Dispersion](intelligent-computing-and-systems/metaheuristics/minimum-differential-dispersion/)

C++ implementations of local, evolutionary, memetic and trajectory-based search for a subset-selection problem. The archive includes benchmark inputs and saved fitness/time comparisons across 50 instance identifiers.

**Focus:** combinatorial optimization, algorithm comparison and seeded experimentation.

**Tools:** C++, CMake.

### [Parchis Search Agent](year-3-ugr/artificial-intelligence/parchis-agent/)

C++ game-playing logic combining minimax, alpha-beta pruning, move ordering and domain-specific heuristics. The implementation explores search depth and node-budget constraints; the original course engine is required to run it.

**Focus:** adversarial search, heuristic design and decision-making.

**Tools:** C++.

### [Statistical Learning on Sensor Data](intelligent-computing-and-systems/machine-learning/)

Classification of meat quality from gas-sensor measurements and regression of pH from electronic-tongue data. Notebooks compare linear, kernel and ensemble methods, with preprocessing, cross-validation and saved evaluation tables. Additional work covers clustering, association rules and autoencoders.

**Focus:** statistical learning, model comparison and experimental interpretation.

**Tools:** Python, scikit-learn, pandas, TensorFlow/Keras.

### [Deep Learning for Computer Vision](intelligent-computing-and-systems/computer-vision/deep-learning/)

CNN configuration comparisons, ResNet34 transfer learning for radiographic age regression, and Grad-CAM examples. Saved test accuracy on a 50-class CIFAR100 subset is 36.16% for BaseNet and 65.32% for the refined configuration; the notebook documents the changes and evaluation setting.

**Focus:** neural-network experiments, transfer learning and visual explanations.

**Tools:** Python, PyTorch, fastai, torchvision.

### [Feature Matching and Image Stitching](intelligent-computing-and-systems/computer-vision/feature-matching-and-stitching/)

Harris detection, SIFT/texture/deep-descriptor matching and a panorama pipeline using RANSAC homographies. Saved examples compare the custom pipeline with OpenCV Stitcher and discuss alignment and seam artifacts in the corridor sequence.

**Focus:** image geometry, robust estimation and model limitations.

**Tools:** Python, OpenCV, NumPy, scikit-image, TensorFlow/Keras.

## Coursework Index

| Academic group | Areas represented |
|---|---|
| [Second year](year-2/) | Algorithm design, data structures, object-oriented programming |
| [Third year — Granada](year-3-ugr/) | Artificial intelligence, server engineering |
| [Third year — Salerno](year-3-salerno/) | Software engineering, parallel programming, databases, geometry, differential equations, probability, topology |
| [Fourth year](year-4/) | Computer graphics |
| [Intelligent Computing and Systems](intelligent-computing-and-systems/) | Optimization, machine learning, computer vision, knowledge engineering, search/planning, language processing, human-computer interaction |

## About the Archive

This is historical academic work. Results are saved experiment outputs, with their settings and limitations explained in the project documentation. They have not been independently reproduced for this portfolio review.

Dependencies and coding conventions reflect the original assignments. Some projects require external data, course frameworks or hardware; there is no repository-wide run command. Original reports, course templates, contributor credits and third-party attributions are retained. Collaborative work is identified in its project documentation.
