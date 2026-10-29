# ESP32-CAM Vision Interface

## 1. Overview

Documents the main ESP32-S3's ESP-NOW interface to an external camera controller. This project does not include that controller's camera or model implementation.

## 2. Hardware and Software Requirements

The main firmware includes Arduino Wi-Fi/ESP-NOW headers. Camera board/module, sensor, model, and camera-side libraries are unknown.

## 3. Architecture

Main sends `START`/`STOP`; remote peer is expected to send prediction strings. Main normalizes and votes over three messages.

## 4. File Structure

- [manager header](../../lib/Esp32Cam_manager/esp32cam_manager.h)
- [manager implementation](../../lib/Esp32Cam_manager/esp32cam_manager.cpp)
- No camera firmware, model, or dataset is included.

## 5. Pin Mapping and Interfaces

Uses ESP-NOW radio, channel 0 peer configuration, encryption disabled. No camera-to-main GPIO mapping is present.

## 6. Implementation

Exact-size `PredictionMessage` packets are accepted while active. Strings containing `GREEN` or `WHITE` count toward a three-result majority; no majority yields unknown. See [pipeline](../../docs/ESP32CAM_INFERENCE_PIPELINE.md).

## 7. Integration with the Main Controller

`main.cpp` starts prediction, waits for the final vote, stops prediction, and selects a servo. The camera-side protocol and inference are unavailable for comparison.

## 8. Setup and Usage

The manager runs from the single main environment; a separately built and matching camera firmware is required but absent. Do not infer the camera setup from this project.

## 9. Testing and Results

No camera peer, model metrics, timing or hardware validation is present.

## 10. Troubleshooting

See [ESP-NOW details](../../docs/ESP32CAM_CONTROLLER_COMMUNICATION.md) and [troubleshooting](../../docs/TROUBLESHOOTING.md). Three missing messages cause an unbounded wait.

## 11. Limitations and Future Improvements

No acknowledgment/retry, sender validation, timeout, protocol versioning or message-layout portability is implemented in the manager.

## 12. Source References

[Controller communication](../../docs/ESP32CAM_CONTROLLER_COMMUNICATION.md) · [model unknowns](../../docs/ESP32CAM_MODEL_IMPLEMENTATION.md) · [vision diagram](../../docs/diagrams/esp32cam_vision_pipeline.mmd)
