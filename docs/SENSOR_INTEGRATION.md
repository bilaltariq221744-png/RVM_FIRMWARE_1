# Sensor Integration

The active application configures one ultrasonic interface: trigger GPIO 12, echo GPIO 13. `getDistance()` emits a short trigger pulse, calls `pulseIn(ECHO, HIGH, 30000)`, returns 999.0 cm for timeout, otherwise converts pulse duration using `0.0343 / 2`. The accept threshold is 25 cm; the same threshold is used to decide that the inlet is clear.

The sensor model, range, mounting, filtering, electrical interface and any echo-level conditioning are not documented. A no-echo timeout is treated as far/clear by the state logic; there is no separate sensor-fault state.

`RVMHardware` additionally has two constructor-defined ultrasonic channels and an IR input where `LOW` means object detected. No instance uses these methods in `main.cpp`; their pins are unknown and they are not part of the active pin table.
