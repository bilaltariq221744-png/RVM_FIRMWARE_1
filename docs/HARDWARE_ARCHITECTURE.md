# Hardware Architecture

The code configures an ESP32-S3 controller and names an external ESP32-CAM peer, DWIN HMI, one ultrasonic sensor and two servos. The serial HMI and sensor/servo GPIO roles are explicit; exact peripheral models, board variants, mechanical assembly and supply design are not present.

`RVMHardware` provides separate APIs for two ultrasonic channels, an IR input, conveyor and crusher outputs, and two servos. No application instance calls it, so it must be treated as optional/unintegrated code rather than the live machine architecture.

No schematic, PCB files, BOM, wiring harness, motor driver model, power tree, independent emergency stop or safety interlock is included. Review [interfaces](HARDWARE_INTERFACES.md), [pinout](HARDWARE_PINOUT.md), and [power unknowns](POWER_DISTRIBUTION.md) before physical integration.
