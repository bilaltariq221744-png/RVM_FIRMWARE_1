# System Overview

The checked-in application is a single Arduino firmware target for an ESP32-S3. It reads a DWIN HMI, polls one ultrasonic sensor, controls two sorting servos, and communicates with an external ESP32-CAM via ESP-NOW.

When started from the HMI, the application accepts bottle detections below 25 cm, asks the camera peer for predictions, and waits for three results. A green or white majority causes the corresponding servo to move and adds Rs 10 to volatile counters. The HMI can then show the count/earning, an organization page, or a payload string on its QR page.

This source does not establish the camera's model/inference implementation, UI widget configuration, mechanical/electrical design, or a real payment flow. Refer to the [project audit](PROJECT_AUDIT.md) and [system diagram](diagrams/system_block_diagram.mmd).
