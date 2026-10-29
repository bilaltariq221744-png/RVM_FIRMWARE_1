# DWIN HMI

## 1. Overview

Provides serial page control, button reception, bottle/earning display writes, and a text payload write for the HMI.

## 2. Hardware and Software Requirements

Firmware uses `HardwareSerial(1)` at 115200 baud, 8N1. Display model, resolution, electrical interface and editor version are not present in the project.

## 3. Architecture

The app calls the `DWIN_HMI` helper; the helper frames UART commands and exposes the latest button code. Display page/widget behavior depends on a missing native HMI project.

## 4. File Structure

- [DWIN_HMI.h](../../lib/DWIN_HMI/DWIN_HMI.h) — IDs, VP constants and API.
- [DWIN_HMI.cpp](../../lib/DWIN_HMI/DWIN_HMI.cpp) — parser and UART writes.

## 5. Pin Mapping and Interfaces

ESP32-S3 RX 10 / TX 9; baud 115200; 8N1. Voltage levels and wiring are not established.

## 6. Implementation

Page IDs 0-9; VPs `0x1001`, `0x1002`, `0x1010`. See [protocol](../../docs/DWIN_UART_PROTOCOL.md), [VP map](../../docs/DWIN_VP_ADDRESS_MAP.md), and [pages](../../docs/DWIN_UI_PAGES.md).

## 7. Integration with the Main Controller

`setup()` calls `begin()`. Each `loop()` calls `update()` then `getButton()`. App button actions select pages and update numeric/text VPs.

## 8. Setup and Usage

Keep the HMI's configured baud, page IDs, VP addresses and touch return frames consistent with firmware. Native project files are needed to reproduce the panel configuration.

## 9. Testing and Results

No display hardware or frame-level test is included. Current build did not reach compilation.

## 10. Troubleshooting

Check RX/TX mapping and baud; compare received bytes to [UART protocol assumptions](../../docs/DWIN_UART_PROTOCOL.md). The parser reads button at offset 8 and does not validate the VP address.

## 11. Limitations and Future Improvements

No model/UI assets, reset pin, timeout, checksum, minimum-length validation or page readback is documented. Avoid changing framing until verified against the actual display project.

## 12. Source References

[HMI implementation](../../docs/DWIN_HMI_IMPLEMENTATION.md) · [UART protocol](../../docs/DWIN_UART_PROTOCOL.md) · [system diagram](../../docs/diagrams/hmi_command_flow.mmd)
