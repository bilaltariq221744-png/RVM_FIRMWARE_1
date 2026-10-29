# Hardware Interfaces

| Link | Source configuration | Source does not establish |
|---|---|---|
| DWIN HMI to ESP32-S3 | UART1, 115200 baud, 8N1; S3 RX 10, TX 9 | DWIN model, connector, voltage levels, reset wiring, cable limits |
| Ultrasonic sensor to ESP32-S3 | TRIG 12 output, ECHO 13 input; 25 cm software threshold | Sensor model, echo voltage, level shifting, mounting/measurement geometry |
| Servo outputs | GPIO 48 and 4; 50 Hz, 500-2400 us attach range; home 0, sort 70 degrees | Servo model, torque/current, supply, mechanical linkage or safe travel |
| ESP32-S3 to camera controller | ESP-NOW; station mode, peer configured on channel 0, encryption disabled | Camera firmware, matching protocol/build, RF environment or link guarantees |
| Optional `RVMHardware` API | Constructor-supplied GPIO for conveyor, crusher, two sonar pairs, IR | Any actual pin values, driver circuitry or use in the integrated firmware |

Do not treat a GPIO assignment as an electrical wiring prescription. Confirm board pinout, peripheral datasheets, logic-level compatibility, power/current budgets, grounding and protection before connecting hardware. See [pinout](HARDWARE_PINOUT.md) and [power distribution](POWER_DISTRIBUTION.md).
