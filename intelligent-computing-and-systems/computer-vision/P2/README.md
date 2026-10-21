# Deep Learning for Computer Vision

CNN classification, transfer learning for radiographic age regression, and visual explanations of image classifiers.

## Approach and implementation

- [CNN experiments](P2_Ejs1y2_Template.ipynb): BaseNet and improvements on a 50-class CIFAR100 subset; includes CPU/GPU comparison and fully connected layer experiments.
- [Transfer learning](P2_Ej3_Template.ipynb): ResNet34 trained from scratch, used as a frozen feature extractor, and fine-tuned for the SPR X-Ray Age Prediction task.
- [Explainability](P2_Ej4_Template.ipynb): Grad-CAM and EfficientNet-B0, with CUB bird classification examples.

## Recorded results

These are saved notebook outputs, not new runs:

| Experiment | Recorded test result | Evidence |
|---|---|---|
| CIFAR100 subset, final improvement evaluation | Accuracy 0.6532; F1 0.6504 | CNN notebook, final test evaluation |
| ResNet34, scratch evaluation | MAE 7.9693; R² 0.7277 | Transfer notebook, test output |
| ResNet34, frozen-feature evaluation | MAE 6.6329; R² 0.8176 | Transfer notebook, test output |
| ResNet34, fine-tuning evaluation | MAE 5.8631; R² 0.8570 | Transfer notebook, test output |
| Bird classifier | Accuracy 0.7447 | Explainability notebook, test output |

The archived age-regression evaluations favor fine-tuning over scratch training for this experiment. These results do not establish general performance across datasets or seeds. The transfer notebook also contains a later classification-style evaluation; it should not be confused with the regression results above.

## Running the notebooks

The code uses Python, PyTorch/fastai, torchvision and supporting numerical/plotting libraries. Inspect each notebook's setup and import cells; obtain its datasets and pretrained weights and restore the expected paths. Saved experiments use Colab/Linux paths. No dependency lockfile or verified end-to-end environment is included; training was not rerun during documentation.

## Academic context

**Course:** Computer Vision, Intelligent Computing and Systems track. **Student identified in notebooks:** Jaime Corzo Galdo. Double Degree in Mathematics and Computer Science, University of Granada. Original exercise text and credits are retained.
