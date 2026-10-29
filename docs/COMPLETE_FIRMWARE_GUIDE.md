# Complete Firmware Guide

## Entry point and initialization

`setup()` in [main.cpp](../src/main.cpp) starts the debug serial port, configures the ultrasonic trigger/echo pins, starts DWIN UART1 at 115200 baud on RX 10/TX 9, attaches two 50 Hz servos (GPIO 48/4) with 500-2400 us pulse bounds and sends both to 0 degrees. It then initializes `ESP32CamManager`, clears transaction counters and updates the HMI data VPs, switches to page 0, and sets the state to `IDLE`.

The manager uses Wi-Fi station mode and ESP-NOW. Initialization failures are printed but are not returned to `setup()` as a startup error.

## Loop and workflow

Every loop calls `hmi.update()` and consumes one pending button. Button handling may change the state/page. The same loop then switches on the current state:

1. `IDLE`: waits for Start.
2. Start from idle resets count/earning, selects page 2 and enters `ACCEPTING`.
3. `ACCEPTING`: reads the ultrasonic sensor; distance below 25 cm enters `DETECTING`.
4. `DETECTING`: starts camera predictions, delays 500 ms, then blocks for exactly three new predictions. It sends stop after collecting the result. A green majority moves servo 1; a white majority moves servo 2. Both move to 70 degrees for one second, return to 0, and add one bottle/Rs 10. No majority adds nothing.
5. The code enters `WAITING_FOR_CLEAR`; distance at least 25 cm returns to `ACCEPTING`.
6. Withdraw selects page 1 and writes the current amount and generated transaction ID as text to VP `0x1010`. Donate selects page 3. Named organization buttons select pages 4-7.
7. Earning-done selects page 8. Thank-you-done or Cancel resets counters and returns to page 0/`IDLE`.

`SORTING` is a declared state but the servo action is performed synchronously inside `DETECTING`; its switch case only advances to `WAITING_FOR_CLEAR`. `WITHDRAW` and `DONATING` states idle while the HMI owns further page navigation. There is no persistent storage or backend.

## Error, timing and recovery

- Ultrasonic `pulseIn` timeout is 30,000 us; a timeout returns 999 cm.
- Camera response collection has no timeout, so missing predictions block the main loop.
- HMI and ESP-NOW initialization/send failures are logged in helper code, but no explicit application recovery state is used.
- Transaction values live only in RAM and reset at explicit transaction completion/cancel/start.

See [state diagram](diagrams/machine_state_machine.mmd), [sequence diagram](diagrams/machine_sequence.mmd), [DWIN protocol](DWIN_UART_PROTOCOL.md), and [camera link](ESP32CAM_CONTROLLER_COMMUNICATION.md).

## Function/module responsibility

| Function/module | Caller and effect |
|---|---|
| `getDistance()` | State loop; triggers/reads the single ultrasonic interface and returns cm or sentinel 999. |
| `changeState()` | Setup and button/state logic; updates current enum and logs its name. |
| `resetTransaction()` / `addBottle()` | Button actions / accepted prediction; update RAM counters and DWIN numeric VPs. |
| `DWIN_HMI::update()` / `getButton()` | Called each loop; parses incoming bytes and exposes the latest button once. |
| `DWIN_HMI::pageSwitch()` / `setData()` / `setQR()` | Called by app; sends HMI commands/data. `setQR` writes text; no payment network action. |
| `ESP32CamManager::startPrediction()` / `getFinalPrediction()` / `stopPrediction()` | Called during detection; sends ESP-NOW commands and returns the three-message vote. |
| `RVMHardware` | Separate API; no caller in the integrated application. |

## Validation boundary

This guide describes source flow, not verified machine behavior. The ESP32-CAM program and HMI configuration are missing, the current build was blocked before compile, and no hardware test results are present.
