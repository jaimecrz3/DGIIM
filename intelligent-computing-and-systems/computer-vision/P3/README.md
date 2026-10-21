# Feature Matching and Image Stitching

[Completed notebook](P3_template.ipynb) implementing Harris corners, descriptor matching and homography-based panorama construction.

## Approach

Compare SIFT, Haralick texture descriptors and deep features; use feature correspondences for image registration and mosaics. The notebook includes ETSIIT and corridor panorama exercises, OpenCV keypoint visualization and non-maximum suppression via `corner_peaks`.

## Results and conclusions

The Harris discussion records **503** detected keypoints with `min_distance = 7` and `threshold_rel = 0.15`, within the assignment's target range of 475–525. The parameter sweep documents how distance and threshold suppress weaker or neighboring detections. This is a result on that notebook image, not a general detector-accuracy measure. Saved figures remain embedded in the notebook.

## Running and technologies

Python, OpenCV, NumPy, scikit-image, mahotas and TensorFlow/Keras (ResNet50 features). Restore images and any expected Colab/Drive paths using the notebook's loading cells. The original environment is not locked; execution was not repeated during this refactor.

## Academic context

Computer Vision, Intelligent Computing and Systems track, Double Degree in Mathematics and Computer Science, University of Granada. The notebook names Jaime Corzo Galdo and retains the course template.
