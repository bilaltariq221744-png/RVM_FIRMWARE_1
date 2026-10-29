# Motor and Actuator Control

## 1. Overview

The active app controls two sorting servos. Other conveyor/crusher helper methods exist but are not used in the integrated app.

## 2. Hardware and Software Requirements

`ESP32Servo` is declared in `platformio.ini`. Servo models, external drivers and power requirements are not provided.

## 3. Architecture

Servo motion runs inline after a green/white camera result. `RVMHardware` is a separate optional abstraction.

## 4. File Structure

- [main.cpp](../../src/main.cpp) — active servo setup and sorting.
- [RVMHardware source](../../lib/RVMHardware/RVMHardware.cpp) — unreferenced conveyor, crusher and servo helper methods.

## 5. Pin Mapping and Interfaces

Servo 1 GPIO 48; servo 2 GPIO 4; 50 Hz and 500-2400 us attach range. Boot log labels are reversed. See [pinout](../../docs/HARDWARE_PINOUT.md).

## 6. Implementation

Both servos home at 0 degrees; green uses servo 1, white servo 2; target is 70 degrees for one second before returning home.

## 7. Integration with the Main Controller

Servos run only after manager returns a majority color. Accepted items increment volatile transaction counters.

## 8. Setup and Usage

Build via the main PlatformIO environment. Confirm mechanical range, board pin capabilities, supply, and current needs on the actual hardware before actuation.

## 9. Testing and Results

No servo model or physical motion verification is recorded.

## 10. Troubleshooting

Check actual constants, not the reversed startup labels. Review [actuator integration](../../docs/ACTUATOR_INTEGRATION.md).

## 11. Limitations and Future Improvements

No nonblocking motion, position feedback, fault detection, safe travel calibration or motor-driver integration is shown.

## 12. Source References

[Actuator documentation](../../docs/ACTUATOR_INTEGRATION.md) · [firmware guide](../../docs/COMPLETE_FIRMWARE_GUIDE.md)
