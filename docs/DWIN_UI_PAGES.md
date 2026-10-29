# DWIN UI Pages

Page IDs are defined in `lib/DWIN_HMI/DWIN_HMI.h`. Titles and likely purpose are based only on identifiers and navigation calls; exact widgets, button coordinates, visual design and touch configuration cannot be established without the native DWIN project.

| ID | Constant | Source-supported role |
|---:|---|---|
| 0 | `PAGE_START` | Startup/return page; Start button is handled by firmware. |
| 1 | `PAGE_QR` | Withdraw page; firmware writes the payload string to VP `0x1010`. |
| 2 | `PAGE_MAIN` | Selected when bottle acceptance begins; count/earning VPs updated. |
| 3 | `PAGE_DONATION` | Selected by Donate. |
| 4 | `PAGE_ALKHIDMAT` | Organization-named page. |
| 5 | `PAGE_EDHI` | Organization-named page. |
| 6 | `PAGE_AIRUNI` | Organization-named page. |
| 7 | `PAGE_HHRD` | Organization-named page. |
| 8 | `PAGE_THANKYOU` | Selected by earning-done button. |
| 9 | `PAGE_ERROR` | Defined but no use found in the integrated firmware. |

## Button codes

These are firmware enum values. The HMI project's touch-event assignments are not included, so the mapping between a physical UI button and each code cannot be independently checked.

| Code | Firmware enum |
|---:|---|
| `0x00` | `NO_BUTTON` |
| `0x01` | `START_BUTTON` |
| `0x02` | `WITHDRAW_BUTTON` |
| `0x03` | `ALKHIDMAT_BUTTON` |
| `0x04` | `EDHI_BUTTON` |
| `0x05` | `AIRUNI_BUTTON` |
| `0x06` | `HHRD_BUTTON` |
| `0x07` | `Thankyou_DONE_BUTTON` |
| `0x08` | `DONATE_BUTTON` |
| `0x09` | `TRY_AGAIN_BUTTON` |
| `0x10` | `Cancel_Button` |
| `0x11` | `Earn_Done_Button` |

`TRY_AGAIN_BUTTON` is defined but no action was found for it. The `Cancel_Button`, `Earn_Done_Button`, and `Thankyou_DONE_BUTTON` behavior is described in the [complete firmware guide](COMPLETE_FIRMWARE_GUIDE.md). Do not assume the software page map guarantees matching UI project IDs.
