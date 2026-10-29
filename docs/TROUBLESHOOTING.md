# Troubleshooting

| Symptom | Source-grounded checks |
|---|---|
| HMI buttons do not register | Confirm UART1 wiring and 115200 8N1 settings; inspect serial output. The parser assumes the expected frame shape and fixed button offset; compare received bytes with the actual HMI configuration. |
| HMI values/pages are wrong | Compare the HMI project with page IDs and VPs in [DWIN_VP_ADDRESS_MAP.md](DWIN_VP_ADDRESS_MAP.md); native UI files are not present. |
| Detection stalls | `getFinalPrediction()` waits for exactly three new predictions without a timeout. Check ESP-NOW peer/protocol compatibility and the separately maintained camera firmware. |
| Camera peer does not initialize | Check ESP-NOW initialization and peer-add messages in serial output; confirm the configured peer address and radio channel match the camera side. |
| Unexpected bottle classification | Review raw prediction strings, normalization, exact three-result voting, and the camera model/thresholds. Camera inference source and model are absent. |
| Distance remains high | `pulseIn` times out after 30 ms and `getDistance()` returns 999 cm; verify sensor wiring, pulse/echo timing, and electrical compatibility. |
| Wrong servo appears to move | Code assigns servo 1 to GPIO 48 and servo 2 to GPIO 4, while boot diagnostic labels are reversed. |
| Build reports missing package manifest | Verify PlatformIO package installation and package metadata; current documented build attempt failed before source compilation. |

Do not use these checks as a substitute for electrical safety verification or hardware documentation.
