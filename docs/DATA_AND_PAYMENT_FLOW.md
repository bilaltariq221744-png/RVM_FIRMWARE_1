# Data and Payment/Reward Flow

## Implemented reward calculation

`src/main.cpp` keeps `bottleCount` and `earning` as volatile `uint16_t` values. A green or white majority increments count by one and earning by `BOTTLE_REWARD` (10). Unknown/no-majority does not add a reward. Starting/canceling/completing a transaction resets both counters; values are not persisted.

## QR-like payload

On the Withdraw button, the firmware shows page 1 and calls `DWIN_HMI::setQR(earning)`. That method creates a random 32-bit value via `esp_random()`, formats it as hexadecimal, constructs `RVM|<amount>|<transactionID>`, and writes the string to VP `0x1010`. This is a text payload, not QR encoding performed by firmware. Whether the DWIN UI renders it as a QR image is unknown because its native project is absent.

## What is not implemented/evidenced

- No scanner or QR decoding/parser.
- No request to a server, payment gateway or bank.
- No validation or acknowledgment of a transaction, duplicate handling, retry, receipt or user identity.
- No durable transaction log or user credit transfer.
- Donation buttons select organization-named pages; no transfer is performed by the source.

Do not treat the displayed/generated payload as a payment instrument or claim funds were transferred. The payload's transaction field is not shown to be authenticated or globally unique. Avoid publishing real production QR payloads if introduced later.
