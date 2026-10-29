# ESP32-CAM Inference Pipeline

Only the main-controller portion is visible:

1. The main ESP32-S3 sends an ESP-NOW `START` command.
2. The external camera controller is expected to send `PredictionMessage` values.
3. The manager normalizes each prediction string (trim, uppercase, spaces/hyphens to underscores).
4. The manager counts strings containing `GREEN` or `WHITE`; other strings count as unknown.
5. After exactly three messages, at least two matching color results are required for a color result; otherwise it returns unknown.
6. The main application stops prediction and conditionally drives a sorting servo.

Camera acquisition, preprocessing, model invocation, confidence validation, class mapping, and camera-side no-object/error handling are not included. No model quality or latency result is available. The process can wait indefinitely if three messages do not arrive. See [controller message details](ESP32CAM_CONTROLLER_COMMUNICATION.md).
