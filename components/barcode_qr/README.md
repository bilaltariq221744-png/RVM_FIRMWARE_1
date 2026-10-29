# Barcode and QR Interfaces

## 1. Overview

The firmware generates a QR-like text payload for display, but contains no barcode/QR scanner or decoder.

## 2. Hardware and Software Requirements

The DWIN helper uses Arduino `String` and `esp_random()`. No scanner model, optical module, library or external QR service is declared.

## 3. Architecture

Withdraw sends text `RVM|amount|transactionID` to DWIN VP `0x1010`. Any visual QR rendering would be provided by the HMI configuration, which is not included.

## 4. File Structure

- [DWIN setQR implementation](../../lib/DWIN_HMI/DWIN_HMI.cpp)
- [VP map](../../docs/DWIN_VP_ADDRESS_MAP.md)
- No scanner/decoder source exists.

## 5. Pin Mapping and Interfaces

Payload is sent over DWIN UART1 (RX 10/TX 9, 115200 8N1). No scanner interface is defined.

## 6. Implementation

The amount is current `earning`; transaction ID is a random 32-bit value rendered as hex. Firmware sends text bytes, not a QR image or encoded bitmap.

## 7. Integration with the Main Controller

`main.cpp` invokes `hmi.setQR(earning)` on Withdraw; no subsequent scan or validation path exists.

## 8. Setup and Usage

Check the native HMI project for VP/widget QR behavior. No scanner setup can be reproduced from this repository.

## 9. Testing and Results

No QR rendering or scanning test has been performed.

## 10. Troubleshooting

See [QR/reward flow](../../docs/DATA_AND_PAYMENT_FLOW.md) and [DWIN protocol](../../docs/DWIN_UART_PROTOCOL.md).

## 11. Limitations and Future Improvements

Payload is unauthenticated and is not shown to identify a backend transaction. No scanner, decoder, transfer, retry or duplicate handling is implemented.

## 12. Source References

[Data/payment flow](../../docs/DATA_AND_PAYMENT_FLOW.md) · [workflow diagram](../../docs/diagrams/payment_workflow.mmd)
