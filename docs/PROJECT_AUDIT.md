# Project Audit

Audit scope: the PlatformIO project at the repository workspace path, excluding generated `.pio/` build/dependency output. This is a static source/configuration review; it is not evidence of hardware operation.

## 1. Current tree

```text
RVM_FIRMWARE_1/
|-- .gitignore
|-- platformio.ini
|-- .vscode/                 PlatformIO-generated/editor configuration
|-- include/README           PlatformIO starter text; no project headers
|-- lib/
|   |-- DWIN_HMI/            DWIN UART helper
|   |-- Esp32Cam_manager/    ESP-NOW manager on the main controller
|   |-- RVMHardware/         General peripheral wrapper, not used by main.cpp
|   `-- README               PlatformIO starter text
`-- src/main.cpp             Integrated ESP32-S3 application and state logic
```

`.pio/` contains generated build and library dependency files and is ignored by Git. There is no `docs/`, `components/`, native DWIN UI project, ESP32-CAM application, model, schematic, BOM, test source, or hardware test record in the inspected project tree.

## 2. Targets and responsibilities

`platformio.ini` defines only `[env:esp32-s3-devkitc-1]`, using `espressif32`, board `esp32-s3-devkitc-1`, Arduino, `BOARD_HAS_PSRAM`, and `madhephaestus/ESP32Servo`. `src/main.cpp` is the only application entry point. The ESP32-CAM is a remote ESP-NOW peer, not a separately buildable target in this project.

## 3. Significant files

| File | Role evidenced by source |
|---|---|
| [src/main.cpp](../src/main.cpp) | Serial setup, sensor polling, UI button handling, bottle transaction state logic, servo sorting and reward tally. |
| [lib/DWIN_HMI/DWIN_HMI.h](../lib/DWIN_HMI/DWIN_HMI.h) | HMI page/button constants, VP addresses, public helper API. |
| [lib/DWIN_HMI/DWIN_HMI.cpp](../lib/DWIN_HMI/DWIN_HMI.cpp) | UART framing/parser, page switching, VP writes and QR payload string creation. |
| [lib/Esp32Cam_manager/esp32cam_manager.h](../lib/Esp32Cam_manager/esp32cam_manager.h) | Bottle result and ESP-NOW message structures/API. |
| [lib/Esp32Cam_manager/esp32cam_manager.cpp](../lib/Esp32Cam_manager/esp32cam_manager.cpp) | ESP-NOW peer setup, START/STOP messages, prediction collection and three-result majority vote. |
| [lib/RVMHardware/RVMhardware.h](../lib/RVMHardware/RVMhardware.h) | Parameterized optional conveyor, crusher, servo, ultrasonic and IR API. |
| [lib/RVMHardware/RVMHardware.cpp](../lib/RVMHardware/RVMHardware.cpp) | Implementation of that API. No use of `RVMHardware` was found in `src/main.cpp`; do not assume these peripherals are integrated. |
| [platformio.ini](../platformio.ini) | Single build environment and direct library dependency. |
| [.gitignore](../.gitignore) | Ignores PlatformIO output and selected editor files. |

## 4. Dependency map

```text
src/main.cpp
  |-- DWIN_HMI (lib/DWIN_HMI)         page/button handling and display writes
  |-- ESP32CamManager (lib/...)       ESP-NOW commands and predictions
  |-- ESP32Servo (PlatformIO package) two sorting servos
  `-- Arduino core                    UART, GPIO, pulseIn, delays, random value

RVMHardware (lib/RVMHardware)         standalone; not called by main.cpp
```

## 5. Communication paths

- HMI: `HardwareSerial(1)`, 115200 baud, 8N1, ESP32-S3 RX GPIO 10 / TX GPIO 9. Source-defined DWIN command and receive framing is described in [DWIN_UART_PROTOCOL.md](DWIN_UART_PROTOCOL.md).
- Camera peer: Wi-Fi station mode plus ESP-NOW. Main controller sends fixed-size `START`/`STOP` command messages and expects fixed-size prediction messages. The camera implementation is absent; wire compatibility is unverified.
- Sensor and servos: local GPIO; no other active bus is evidenced. No SPI, I2C, Ethernet, scanner UART, SD, storage, or backend API implementation was found.

## 6. Pin assignments found

| Function | GPIO | Evidence / qualification |
|---|---:|---|
| DWIN RX | 10 | `hmi.begin(115200, 10, 9)` |
| DWIN TX | 9 | Same call |
| Ultrasonic trigger | 12 | `src/main.cpp` |
| Ultrasonic echo | 13 | `src/main.cpp` |
| Sorting servo 1 | 48 | `SERVO1_PIN` |
| Sorting servo 2 | 4 | `SERVO2_PIN` |

The boot diagnostic text labels servo 1 as GPIO 4 and servo 2 as GPIO 48, opposite the actual constants. Pin numbering alone does not establish electrical compatibility. See [HARDWARE_PINOUT.md](HARDWARE_PINOUT.md).

`RVMHardware` receives all pin numbers from its constructor, but no instance or pin mapping exists in the application.

## 7. Features and implementation locations

- Ultrasonic detection threshold: 25 cm; `pulseIn` timeout: 30,000 us; timeout becomes 999 cm.
- Bottle classification: manager collects three predictions and accepts a green/white majority of at least two.
- Sorting: green uses servo 1; white uses servo 2; each moves to 70 degrees for one second, then home.
- Reward: each accepted color adds 1 bottle and Rs 10 in volatile `uint16_t` counters.
- UI: page/button/VP definitions and data writes in DWIN helper; user workflow in `main.cpp`.
- QR-like content: firmware constructs a text string and sends it to VP `0x1010`. Whether the display turns that VP into a visual QR symbol is not verifiable without the DWIN project.

No barcode/QR scanner, payment submission/validation, user identity, persistent transaction store, or external payment integration appears in the source.

## 8. Build settings and dependencies

The one build environment is documented in [BUILD_AND_SETUP.md](BUILD_AND_SETUP.md). Local `DWIN_HMI` and `ESP32CamManager` are included by the application; `ESP32Servo` is declared in `lib_deps`. `RVMHardware` is a local library but currently unused by the application.

## 9. Static risks and unresolved facts

- `getFinalPrediction()` waits indefinitely for exactly three new messages; no timeout or cancellation is present.
- ESP-NOW uses a hard-coded peer address and `encrypt = false`; callback data is accepted by payload length and active-cycle flag, not sender address.
- ESP-NOW messages are copied as native C structs; field width/alignment/endianness must match the missing camera firmware.
- Prediction reception is a single shared "latest message" slot, not a queue; a burst can overwrite an unread prediction. The callback also constructs Arduino `String` from a fixed char array without checking for a terminator.
- DWIN parser checks sync and command byte but reads the button at fixed offset 8 without checking the frame's minimum payload length or VP address.
- DWIN string writer uses a fixed 150-byte stack buffer without checking input length. Current generated QR text is short, but the public helper is not generally bounded.
- Main loop and camera collection use blocking delays/loops; responsiveness during detection is limited.
- The servo GPIO values printed at startup are swapped relative to the configured pins.
- Include directives for `Esp32Cam_manager.h` and `RVMHardware.h` differ in letter case from the checked-in header filenames. The current Windows environment is case-insensitive; a case-sensitive build host may fail to resolve these headers.
- Existing `RVMHardware` APIs are not the implementation used by the active application; documentation must keep that distinction.
- Hardware model, logic levels, supplies, servo loads, DWIN page/widget config, camera/model, prediction thresholds, safety interlocks and actual transaction behavior require evidence or owner confirmation.

These are static observations, not a penetration test or a claim that the prototype has failed in operation. No firmware behavior was changed during this audit.

## 10. Repository/Git context

Before this publication, the nearest Git root was the user's home directory rather than the PlatformIO project; it had no commits and its `origin` targeted an unrelated website repository. The enclosing worktree contains unrelated personal files. Publication is being isolated in a new Git root directly inside this project; the enclosing repository and its remote must remain untouched. Review the project-level ignore list and stage only the intended firmware files.
