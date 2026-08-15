# Vendored Arduino libraries

These are **copies**, not Library Manager installs. The Mega sketch includes them by basename (`FAB_LED.h`, `SyncDriver.h`, …).

| Folder | Upstream | Why it’s here |
| --- | --- | --- |
| `FAB_LED/` | [sonyhome/FAB_LED](https://github.com/sonyhome/FAB_LED) | Tiny WS2812 bit-bang; `STATIC_ASSERT` stubbed for AVR |
| `StepperDriver/` | [laurb9/StepperDriver](https://github.com/laurb9/StepperDriver) | **`MAX_MOTORS` 3 → 12** (not upstream). Replacing this folder with a stock install breaks 12-axis `SyncDriver`. |
| `IRremote/` | Arduino-IRremote | Headers pulled in by `IR_sensor_setup.h` |
| `MSGEQ7/` | — | Spectrum chip helper (unused in the live loop) |

Compile with:

```bash
arduino-cli compile --fqbn arduino:avr:mega --libraries firmware/lib firmware/STAEXE
```

Or `./scripts/verify-mega.sh` from the repo root (does that plus SRAM/flash budgets).
