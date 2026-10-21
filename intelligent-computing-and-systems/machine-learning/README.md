# Machine Learning

Python notebooks investigating sensor-data prediction, clustering, association rules and neural-network models, with code, saved outputs and written interpretation.

## Supervised learning on sensor data

[Supervised-learning notebook](supervised-learning.ipynb): classify meat quality from gas-sensor measurements and estimate pH from electronic-tongue data. The workflow explores preprocessing, scaling, dimensionality reduction/feature selection, cross-validation and hyperparameter search. Models include logistic regression, SVC and random forests for classification, and Ridge, SVR and random forests for regression.

Saved pH-regression test evaluations:

| Model | R² | MAE |
|---|---:|---:|
| Ridge | 0.8854 | 0.8250 |
| SVR | 0.8844 | 0.8215 |
| RandomForest | 0.9114 | 0.5545 |

RandomForest has the highest test R² of these archived evaluations. Its validation R² is 0.9913, but the later validation subset was drawn after upstream processing and tuning. Preprocessing also precedes model cross-validation rather than being refitted within each fold. These are historical coursework evaluations, not independently validated performance estimates.

## Unsupervised learning

[Unsupervised-learning notebook](unsupervised-learning.ipynb): KMeans and DBSCAN on Old Faithful data, followed by Apriori and FP-Growth association mining. The DBSCAN search records training silhouette 0.5785 at eps 0.1 and min_samples 8. That value describes the archived search setting, including its noise-label handling, rather than predictive accuracy.

## Neural networks and reconstruction

[Deep-learning notebook](deep-learning.ipynb): Fashion-MNIST classifier comparisons and an MNIST convolutional autoencoder, including optimizer/loss experiments and latent-space sampling. The final saved AdamW/Huber autoencoder evaluation reports test MAE 0.0121 on pixels normalized to [0, 1]. This measures reconstruction error; sampled images are explored qualitatively.

## Reading and running

Start with each notebook's setup, imports and data-loading cells. Dependencies include NumPy, pandas, scikit-learn, Matplotlib and TensorFlow/Keras; association mining has additional notebook-specific imports. External data and original Colab/Drive paths must be restored. There is no complete locked environment or independently reproduced run supplied with this archive.

## Academic context

Machine Learning, Double Degree in Mathematics and Computer Science, University of Granada. The notebooks identify Jaime Corzo Galdó and retain the original course templates and credits.
