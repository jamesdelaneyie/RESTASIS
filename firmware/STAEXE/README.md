# STAEXE firmware (Arduino Mega 2560)

Open this folder in Arduino IDE **or** compile from the repo root:

```bash
./scripts/verify-mega.sh      # Mega 2560 + flash/SRAM budgets
./scripts/export-firmware.sh  # dist/STAEXE/ + .hex + zip
```

Board: **Arduino Mega 2560**. Entry: `STAEXE.ino`.

Vendored libs live in `../lib/` (FAB_LED, 12-motor StepperDriver fork, IRremote, MSGEQ7). Do not install stock StepperDriver from Library Manager.

## Microstepping

`setup_motors.h` has `#define MICROSTEPS 8`. That **must match** the A4988 MS1/MS2/MS3 straps on the PCB (currently 1/8). Changing only the define will scale every `rotate()` angle wrongly.

## Memory

One 70-pixel GRB buffer is mirrored to all 18 strips (~210 bytes). Last verify: flash ~7%, SRAM ~20% of the Mega.
