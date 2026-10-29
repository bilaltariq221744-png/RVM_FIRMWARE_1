# Communication Protocols

## DWIN UART

Firmware starts UART1 at 115200 baud, 8N1 (RX GPIO 10, TX GPIO 9). Outbound page writes use a `5A A5` header and the DWIN page-control command; numeric and string VP writes use command `0x82`. The receive parser expects `5A A5`, then a length byte, and recognizes command `0x83`; it reads a button byte at buffer offset 8. It does not validate the returned VP address. See [UART frames](DWIN_UART_PROTOCOL.md).

## ESP-NOW

The ESP32-S3 switches to station mode, initializes ESP-NOW, registers a receive callback, and adds a peer on channel 0 with encryption disabled. `CommandMessage` contains `char command[10]`; `PredictionMessage` contains `char prediction[20]` and `unsigned long timestamp`. Start/stop sends the whole command struct. Receive processing requires exact `sizeof(PredictionMessage)` and an active prediction cycle.

The camera source is absent, so the raw struct ABI, device identity, prediction vocabulary and message timing are not cross-validated. There is no acknowledgment/retry protocol in the manager.

No other active network or wired protocol implementation is present in this project.
