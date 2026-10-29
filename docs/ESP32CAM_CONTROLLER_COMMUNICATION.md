# ESP32-CAM Controller Communication

The ESP32-S3 manager selects Wi-Fi station mode, initializes ESP-NOW, registers a receive callback, and adds a peer on channel 0 without encryption. The peer address is hard-coded in source; its literal value is intentionally not repeated in public-facing documentation.

| Direction | C++ message | Fields |
|---|---|---|
| Main controller to camera | `CommandMessage` | `char command[10]`, text `START` or `STOP`, remaining bytes zeroed |
| Camera to main controller | `PredictionMessage` | `char prediction[20]`, `unsigned long timestamp` |

The sender's `esp_now_send()` result is checked for immediate enqueue status only. There is no acknowledgment protocol, retry, authentication or sender-MAC check. The receive callback accepts only exact `sizeof(PredictionMessage)` packets while a prediction cycle is active. The native structure layout and `unsigned long` width must match on both targets; no packing/versioning is defined. The camera source is absent, so interoperability is not verified.

The main application waits for exactly three new received messages without a timeout. No timestamp age/order validation is performed by the manager.
