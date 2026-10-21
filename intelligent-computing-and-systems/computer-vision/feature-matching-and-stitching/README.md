# Feature Matching and Image Stitching

A Python notebook implementing feature detection, descriptor matching and panorama construction through homography estimation.

[Open the notebook](feature-matching-and-stitching.ipynb) for the code, saved figures and discussion.

## Approach

- Harris corner responses and non-maximum suppression with `corner_peaks`.
- SIFT, Haralick texture and pretrained ResNet50 patch descriptors.
- Brute-force, mutual cross-check and Lowe ratio-test matching.
- RANSAC homography estimation, composition into a common canvas and image warping.
- ETSIIT and corridor panoramas, compared with OpenCV Stitcher.

## Saved observations

The Harris discussion records 503 keypoints at `min_distance = 7` and `threshold_rel = 0.15` on the assignment image. This is a parameter-setting observation, not a detector-accuracy measurement.

The notebook discussion favors SIFT correspondences over the tested texture and deep-patch descriptors on the example image pair. No ground-truth matching-accuracy score is reported.

Saved mosaics and the final discussion compare the custom pipeline with OpenCV Stitcher. The discussion reports cleaner transitions from OpenCV, particularly for the corridor. It relates the custom pipeline's alignment difficulties to parallax and accumulated homography error, illustrating the limitations of the chosen geometric model.

## Environment

Python, OpenCV, NumPy, scikit-image, mahotas and TensorFlow/Keras. The notebook uses ResNet50 pretrained weights and external image inputs; inspect its loading cells and original Colab/Drive paths before execution. The archive retains saved outputs but does not supply a complete locked environment or an independently reproduced run.

## Academic context

Computer Vision, Intelligent Computing and Systems track, Double Degree in Mathematics and Computer Science, University of Granada. The notebook identifies Jaime Corzo Galdó and retains the original course template.
