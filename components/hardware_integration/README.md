# Hardware Integration

## 1. Overview

Records GPIO interfaces found in the integrated firmware and separates them from optional APIs that are not called.

## 2. Hardware and Software Requirements

The declared controller is `esp32-s3-devkitc-1`; exact peripheral models and electrical requirements are not provided.

## 3. Architecture

Active firmware configures an HMI UART, one ultrasonic sensor, two servo outputs and an ESP-NOW peer. `RVMHardware` also describes optional outputs/sensors but is not instantiated.

## 4. File Structure

- [main.cpp](../../src/main.cpp) — active pin mappings.
- [RVMHardware API](../../lib/RVMHardware/RVMhardware.h) and [implementation](../../lib/RVMHardware/RVMHardware.cpp) — standalone wrapper.
- [Hardware docs](../../docs/HARDWARE_ARCHITECTURE.md).

## 5. Pin Mapping and Interfaces

See [pin table](../../docs/HARDWARE_PINOUT.md) and [interface table](../../docs/HARDWARE_INTERFACES.md). The printed startup servo labels are reversed from code assignments.

## 6. Implementation

GPIO initialization and active sensor/servo actions occur directly in `main.cpp`. No conveyor or crusher GPIO values are assigned in the application.

## 7. Integration with the Main Controller

The ultrasonic reading affects state transitions; servo outputs sort only majority-confirmed bottle classes. Electrical design is not captured.

## 8. Setup and Usage

Use the source pin map only as a signal inventory. Obtain board/peripheral pinouts and verify levels, power and safe mechanics before wiring.

## 9. Testing and Results

No schematic review, electrical checks or physical hardware tests were recorded.

## 10. Troubleshooting

See [sensor](../../docs/SENSOR_INTEGRATION.md), [actuator](../../docs/ACTUATOR_INTEGRATION.md) and [power](../../docs/POWER_DISTRIBUTION.md) notes.

## 11. Limitations and Future Improvements

Models, supply rails, drivers, power budget, connector mapping and safety interlocks remain unknown.

## 12. Source References

[Pinout](../../docs/HARDWARE_PINOUT.md) · [hardware diagram](../../docs/diagrams/hardware_interfaces.mmd) · [audit](../../docs/PROJECT_AUDIT.md)
