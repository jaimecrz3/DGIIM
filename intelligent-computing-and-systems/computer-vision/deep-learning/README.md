# Deep Learning for Computer Vision

Python experiments on CNN classification, transfer learning for radiographic age regression and Grad-CAM visualization, documented in three notebooks.

## CNN configuration comparisons

[CNN notebook](cnn-classification.ipynb): BaseNet, a variant without fully connected layers and a refined configuration on a 50-class CIFAR100 subset.

| Configuration | Saved test accuracy | Macro-F1 |
|---|---:|---:|
| BaseNet | 36.16% | 0.3545 |
| Without fully connected layers | 19.70% | 0.1586 |
| Refined configuration | 65.32% | 0.6504 |

The refined configuration changes architecture and training choices together. These evaluations support a comparison of the archived configurations, not a causal attribution to one change.

## Transfer learning

[ResNet34 notebook](transfer-learning.ipynb): compare training from scratch, frozen pretrained features and fine-tuning for the SPR X-Ray Age Prediction task.

| Training strategy | Saved test MAE | Saved test R² |
|---|---:|---:|
| From scratch | 7.9693 | 0.7277 |
| Frozen features | 6.6329 | 0.8176 |
| Fine-tuning | 5.8631 | 0.8570 |

Fine-tuning has the lowest MAE of these saved evaluations. MAE is reported in the dataset's age-label units. A later age-classification experiment in the notebook is a separate formulation and is not part of this regression comparison.

## Visual explanations

[Grad-CAM notebook](grad-cam.ipynb): an EfficientNet-B0 classifier on CUB bird images, with saved attention-map examples. Its saved test accuracy is 74.47%; that score measures classification, not explanation faithfulness.

## Environment and execution

Python, PyTorch, fastai and torchvision, with notebook-specific numerical, plotting and CAM libraries. Read the setup/data-preparation cells first: some packages are version-pinned, external images or pretrained weights are required, and preparation cells remove or recreate downloaded-data folders. Original Colab/Linux paths remain. A complete locked environment and independently reproduced run are not supplied here.

All numbers above are archived notebook outputs; they do not represent repeated-run estimates or performance across other datasets.

## Academic context

Computer Vision, Intelligent Computing and Systems track, Double Degree in Mathematics and Computer Science, University of Granada. The notebooks identify Jaime Corzo Galdó and retain the course templates and credits.
