# DWIN UART Protocol

## Configuration

Firmware uses `HardwareSerial(1)`, baud `115200`, `SERIAL_8N1`, RX GPIO 10, TX GPIO 9. No reset pin, flow control, voltage-level details or display model is specified.

## Source-generated outbound frames

| Purpose | Frame layout |
|---|---|
| Page switch | `5A A5 07 82 00 84 5A 01 00 <page>` |
| 16-bit numeric VP write | `5A A5 05 82 <VP high> <VP low> <value high> <value low>` |
| Text VP write | `5A A5 <text bytes + 3> 82 <VP high> <VP low> <text bytes...>` |

`writeNumber()` writes a 16-bit value in big-endian byte order. `writeString()` sends the raw Arduino `String` bytes, without an added terminator or Unicode conversion. Only short ASCII payloads are currently constructed by `setQR()`.

## Receive behavior

`update()` accumulates bytes until it sees header `5A A5` and the declared frame length. `parseFrame()` only proceeds when byte 3 is `0x83`, then reads the button code at byte 8. The parser does not check the variable address or enforce a minimum frame length before that offset read. There is no checksum or timeout validation in this helper.

The exact incoming packet fields and touch configuration must be checked against the missing DWIN project. The parser's assumptions are not a complete DWIN protocol specification.
