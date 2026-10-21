# Machine Learning

Three completed notebooks covering supervised learning, clustering and association rules, deep classification and autoencoders. Saved outputs and discussion remain in the notebooks.

## Contents

- [deep-learning.ipynb](deep-learning.ipynb)
- [supervised-learning.ipynb](supervised-learning.ipynb)
- [unsupervised-learning.ipynb](unsupervised-learning.ipynb)

## Technologies and formats

Python; NumPy; pandas; scikit-learn; Matplotlib; TensorFlow/Keras.

## Academic context

Double Degree in Mathematics and Computer Science, University of Granada. Original source, reports, templates and contributor credits are retained.

## Supervised learning: electronic sensor data

[supervised-learning.ipynb](supervised-learning.ipynb) addresses meat-quality classification from gas sensors and pH regression from an electronic tongue. The implemented workflow includes exploratory analysis, scaling/feature selection, model selection and validation/test evaluation. Classification compares logistic regression, SVC and random forests; regression compares Ridge, SVR and random forests.

**Recorded result:** the RandomForest regression test output reports **R² = 0.9114068976935454**, versus its validation R² of **0.9912636194671365**. The gap is a reason to distinguish validation fit from held-out performance. These are archived outputs for the notebook's pH experiment, not independently reproduced metrics.

## Unsupervised learning: clustering and association rules

[unsupervised-learning.ipynb](unsupervised-learning.ipynb) applies KMeans and DBSCAN to the Old Faithful clustering task, and Apriori/FP-Growth to frequent itemsets and association rules. It records silhouette measurements for multiple configurations, including **0.5785** in a best-configuration output. Consult the neighboring code and preprocessing choices before comparing it with other saved scores: silhouette depends on the data representation and cluster assignment.

## Deep learning: classification and reconstruction

[deep-learning.ipynb](deep-learning.ipynb) compares classifier architectures and trains an MNIST convolutional encoder/decoder. Its experiments explore optimizer/loss choices and reconstruct digit images, with latent-space sampling examples. The final saved autoencoder test output reports **MAE = 0.012067900970578194**. The accompanying discussion selects AdamW with Huber loss for the reconstruction experiment. Reconstruction error alone does not establish the quality of generated samples.

## Running the notebooks

Open the selected notebook in Jupyter or the original Colab environment and inspect its setup, imports and data-loading cells before execution. Dependencies vary: supervised learning uses scikit-learn/pandas/NumPy; deep learning adds TensorFlow/Keras; association mining has its own imports. Datasets and original external/Google Drive paths are not bundled into a reproducible environment. No pinned environment is supplied, and training was not rerun during this refactor. Notebook filenames were changed; cell source, saved outputs and original student/template credits were preserved byte-for-byte. The notebooks identify Jaime Corzo Galdo as the student.
