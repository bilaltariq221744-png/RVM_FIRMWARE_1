# Build and Setup

## Environment

The only declared environment is `esp32-s3-devkitc-1` in [platformio.ini](../platformio.ini):

- Platform: `espressif32`
- Board: `esp32-s3-devkitc-1`
- Framework: Arduino
- Build flag: `-DBOARD_HAS_PSRAM`
- Declared external library: `madhephaestus/ESP32Servo`
- Serial monitor: 115200 baud; upload speed: 921600

Local libraries under `lib/` are discovered by PlatformIO. There is no ESP32-CAM environment in this project.

## Commands

```powershell
pio run -e esp32-s3-devkitc-1
pio run -e esp32-s3-devkitc-1 -t upload
pio device monitor -b 115200
```

The application expects the external camera peer and DWIN UI to be separately configured. Exact camera setup and native HMI project files are not included. Hardware wiring must be confirmed independently; see [pinout](HARDWARE_PINOUT.md).

## Current workstation result

The installed PlatformIO Core 6.2.0 was invoked directly because `pio` was not on the PowerShell path. Build exited before compilation with `MissingPackageManifestError: Could not find one of 'package.json' manifest files in the package`. No dependency was changed or restored. See [validation report](REFACTOR_AND_VALIDATION_REPORT.md).
