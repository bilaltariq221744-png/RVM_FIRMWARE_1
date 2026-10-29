# Actuator Integration

The integrated application creates two `ESP32Servo::Servo` objects. Both are set to 50 Hz, attached with 500-2400 us pulse bounds, and initialized at 0 degrees. For a green prediction, servo 1 (GPIO 48) moves to 70 degrees for one second and returns to 0; white uses servo 2 (GPIO 4) with the same sequence. A confirmed color then increments bottle count and earning.

No servo model, torque, supply, linkage, end-stop, load, or mechanical safety evidence is present. GPIO numbers and pulse bounds alone do not prove compatibility or safe motion.

`RVMHardware` contains separate servo helpers with incremental 1-degree steps and delayed behavior, but the active application does not instantiate this class and instead calls `Servo::write()` directly. Its conveyor/crusher output methods are likewise not used by the integrated firmware.
