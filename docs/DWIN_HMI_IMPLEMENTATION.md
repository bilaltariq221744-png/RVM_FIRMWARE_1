# DWIN HMI Implementation

The application constructs `HardwareSerial DWIN(1)` and passes it to `DWIN_HMI`. Setup calls `hmi.begin(115200, 10, 9)`, configuring 8N1 with ESP32-S3 RX 10 and TX 9. `loop()` calls `update()` and then consumes the latest button with `getButton()`. Button handling is in `src/main.cpp`; framing, page switching and VP writes are in `lib/DWIN_HMI/`.

The helper defines ten page IDs, twelve button IDs and three VP addresses. It sends `0x82` writes for page control, numeric values and text. Its receive parser looks for `5A A5`, accepts command `0x83`, and reads the button value at byte offset 8.

No DWIN model, resolution, panel electrical interface, native project file, screenshots, touch-area settings or widget configuration is included. Page/widget purposes below are source labels and workflow inferences, not verified UI design. See [UART frames](DWIN_UART_PROTOCOL.md), [VP map](DWIN_VP_ADDRESS_MAP.md), and [page map](DWIN_UI_PAGES.md).
