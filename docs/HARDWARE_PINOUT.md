# Hardware Pinout

Mappings below are taken from the integrated application. Confirm the specific ESP32-S3 board pinout and electrical conditions before wiring.

| ESP32-S3 GPIO | Direction in firmware | Function | Configuration |
|---:|---|---|---|
| 10 | RX | DWIN UART1 | 115200 baud, 8N1 |
| 9 | TX | DWIN UART1 | 115200 baud, 8N1 |
| 12 | Output | Ultrasonic trigger | 2 us low, 10 us high pulse |
| 13 | Input | Ultrasonic echo | `pulseIn(HIGH, 30000 us timeout)` |
| 48 | PWM output | Servo 1 | 50 Hz; 500-2400 us; home 0, sort 70 |
| 4 | PWM output | Servo 2 | 50 Hz; 500-2400 us; home 0, sort 70 |

The startup diagnostic prints servo 1 on GPIO 4 and servo 2 on GPIO 48, opposite the code constants. Treat the constants/attach calls as the active assignments and correct/verify diagnostics in a separate code change.

`Serial` debug output uses the Arduino default console configuration at 115200 baud; explicit GPIO mapping is not provided. ESP-NOW uses the radio and has no GPIO mapping. Optional `RVMHardware` pins are constructor arguments and have no values specified in the project.
