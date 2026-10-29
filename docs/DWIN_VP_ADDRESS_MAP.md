# DWIN VP Address Map

The addresses and meanings below are defined by firmware constants. Data width is inferred from the helper that writes to the VP. The native HMI project is absent, so widget assignment and rendered format remain unverified.

| VP | Firmware name | Writer | Format / meaning |
|---:|---|---|---|
| `0x1001` | `VP_BOTTLE` | `writeNumber()` via `setData()` | 16-bit bottle count |
| `0x1002` | `VP_EARNING` | `writeNumber()` via `setData()` | 16-bit earning amount; serial text labels units as Rs |
| `0x1010` | `VP_QR` | `writeString()` via `setQR()` | ASCII `RVM|amount|transactionID` text; visual QR rendering depends on HMI configuration |

There are no other VP definitions in the inspected DWIN helper header. No VP readback is implemented.
