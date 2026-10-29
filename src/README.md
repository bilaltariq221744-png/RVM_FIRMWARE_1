# Integrated Firmware Source

## 1. Overview

`main.cpp` contains the ESP32-S3 application entry point and integrated machine flow.

## 2. Hardware and Software Requirements

The declared target is `esp32-s3-devkitc-1`, Arduino framework, with `ESP32Servo`. Peripheral models are not identified in source.

## 3. Architecture

The application initializes UART HMI, an ultrasonic sensor, two servos, and an ESP-NOW manager. It runs a synchronous state switch from `loop()`.

## 4. File Structure

- `main.cpp` — setup, states, button actions, distance sampling, classification and reward logic.
- Support code is under `../lib/`; see [lib README](../lib/README).

## 5. Pin Mapping and Interfaces

See [hardware pinout](../docs/HARDWARE_PINOUT.md). Active application mappings: DWIN RX/TX 10/9, ultrasonic TRIG/ECHO 12/13, servos 48/4.

## 6. Implementation

The machine accepts distances below 25 cm, waits for three camera predictions, sorts green/white results and adds Rs 10 per accepted bottle. See [complete guide](../docs/COMPLETE_FIRMWARE_GUIDE.md).

## 7. Integration with the Main Controller

This file is the main controller application. It calls `DWIN_HMI`, `ESP32CamManager`, Arduino GPIO APIs and `ESP32Servo`.

## 8. Setup and Usage

Build with `pio run -e esp32-s3-devkitc-1`. The current build environment error is recorded in [validation](../docs/TESTING_AND_VALIDATION.md).

## 9. Testing and Results

No automated or hardware tests are present. Current PlatformIO build attempt failed before compilation.

## 10. Troubleshooting

See [troubleshooting](../docs/TROUBLESHOOTING.md); camera prediction collection can block indefinitely.

## 11. Limitations and Future Improvements

The state flow is synchronous, the `SORTING` state is effectively bypassed by inline servo actions, and camera/HMI integrations have no included peer assets.

## 12. Source References

- [main.cpp](main.cpp)
- [DWIN helper](../lib/DWIN_HMI/DWIN_HMI.h)
- [Camera manager](../lib/Esp32Cam_manager/esp32cam_manager.h)
