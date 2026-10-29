# Firmware Architecture

`src/main.cpp` owns the integrated runtime and contains the state machine, GPIO configuration, transaction counters, HMI button dispatch, bottle classification call and servo actions. `DWIN_HMI` handles display framing and VP/page operations. `ESP32CamManager` owns ESP-NOW initialization, command transmission and prediction voting. The separate `RVMHardware` class is not called by the application.

The main loop is synchronous. It polls the HMI, handles at most the last parsed button, then runs the current machine state. Detection blocks while collecting three camera messages. Sorting actions and most transitions run inline; there is no task/RTOS architecture in project code beyond Arduino framework behavior.

See [complete execution guide](COMPLETE_FIRMWARE_GUIDE.md), [dependency diagram](diagrams/firmware_dependencies.mmd), and the [state diagram](diagrams/machine_state_machine.mmd).
