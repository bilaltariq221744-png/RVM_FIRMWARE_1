# System Architecture

```mermaid
flowchart LR
  HMI[DWIN display / touch UI<br/>model and UI project unknown]
  S3[ESP32-S3<br/>src/main.cpp]
  CAM[ESP32-CAM peer<br/>firmware/model absent]
  US[Ultrasonic sensor<br/>model unknown]
  S1[Servo 1<br/>GPIO 48]
  S2[Servo 2<br/>GPIO 4]
  HMI <-->|UART1 115200 8N1<br/>RX 10 / TX 9| S3
  S3 <-->|ESP-NOW<br/>unencrypted peer| CAM
  S3 <-->|TRIG 12 / ECHO 13| US
  S3 -->|PWM| S1
  S3 -->|PWM| S2
```

The peripherals are identified by source symbols and APIs only; no manufacturer/model or electrical rating is established. The optional `RVMHardware` class describes additional conveyor, crusher, second ultrasonic and IR interfaces but is not instantiated by the integrated application. No power distribution diagram or independent safety controller is included.

See [hardware architecture](HARDWARE_ARCHITECTURE.md), [communication protocols](COMMUNICATION_PROTOCOLS.md), and [editable diagrams](diagrams/).
