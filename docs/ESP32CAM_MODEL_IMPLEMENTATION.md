# ESP32-CAM Model Implementation

This project contains only the ESP32-S3-side `ESP32CamManager` interface. It does not contain ESP32-CAM application source, camera initialization/capture code, a model file, labels, training code, dataset, preprocessing, confidence threshold or evaluation record.

The main controller recognizes the strings `GREEN` and `WHITE` after normalizing received text, then applies a three-message majority vote. That is result handling, not evidence of where or how image inference occurs. The camera module/board variant, model architecture/format, training authorship, accuracy, inference time and memory use are unknown or unmeasured.

Do not claim an embedded-AI model implementation or training result based on the manager interface alone. Obtain and review the separate camera firmware and model provenance before completing this document.
