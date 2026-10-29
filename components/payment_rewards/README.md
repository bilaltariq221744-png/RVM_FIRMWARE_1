# Payment and Bottle Rewards

## 1. Overview

The application tallies a local bottle count and earning amount and sends a text payload to a DWIN VP. It does not implement a payment service.

## 2. Hardware and Software Requirements

Only the ESP32-S3 firmware and HMI interface are visible. No backend, scanner, payment SDK, credentials or storage is declared.

## 3. Architecture

Accepted green/white bottles add Rs 10 in volatile counters. Withdraw displays a payload; no transaction validation or transfer follows.

## 4. File Structure

- [main.cpp](../../src/main.cpp) — count, earning and button workflow.
- [DWIN helper](../../lib/DWIN_HMI/DWIN_HMI.cpp) — payload string creation.
- [Detailed flow](../../docs/DATA_AND_PAYMENT_FLOW.md).

## 5. Pin Mapping and Interfaces

Uses DWIN UART1 and VP `0x1010`; no payment network interface appears.

## 6. Implementation

Payload shape is `RVM|amount|transactionID`. Amount is the accumulated `uint16_t` earning; ID is a random 32-bit hex value. Counters reset at transaction start/cancel/completion.

## 7. Integration with the Main Controller

The firmware updates bottle/earning VPs, then selects the QR page on Withdraw. Organization buttons navigate UI pages only.

## 8. Setup and Usage

Do not treat the payload as a payment receipt or a credit transfer. Verify native HMI rendering before claiming it displays a QR.

## 9. Testing and Results

No payment, donation transfer, persistent credit, backend or user-facing QR test is present.

## 10. Troubleshooting

See [data flow](../../docs/DATA_AND_PAYMENT_FLOW.md), [UI map](../../docs/DWIN_UI_PAGES.md), and [validation status](../../docs/TESTING_AND_VALIDATION.md).

## 11. Limitations and Future Improvements

No persistence, authentication, user identification, backend validation, duplicate handling, retry, or transaction outcome state exists in this source.

## 12. Source References

[Data/payment flow](../../docs/DATA_AND_PAYMENT_FLOW.md) · [firmware guide](../../docs/COMPLETE_FIRMWARE_GUIDE.md)
