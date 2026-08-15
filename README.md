# STAEXE

Kinetic LED sculpture — a hanging pointy-top hexagon of **12 NEMA 17 steppers** and **18 WS2812 tubes**, first shown at **Body & Soul Festival 2022**. Firmware runs on an **Arduino Mega 2560**; a desktop sim plays the same C++ choreography in the browser.

Drop a night shot in `docs/photos/` and link it here.

## The piece

Six vertices, motor **pairs** at each corner. LED tubes ride the motor arms (aimed with the steppers) and sit on the outer struts. Mega + 12× **A4988** (24 V / ~2 A) + FAB_LED bit-bang, one 70-pixel buffer replayed to every strip so SRAM stays tiny.

## Repo map

| Path | What |
| --- | --- |
| [`firmware/STAEXE/`](firmware/STAEXE/) | Flash this (Mega 2560) |
| [`firmware/lib/`](firmware/lib/) | Vendored FAB_LED + 12-axis StepperDriver fork |
| [`sim/`](sim/) | PIXI preview of LEDs + motors |
| [`hardware/`](hardware/) | PCB, gerbers, BOM (drop files here) |
| [`docs/`](docs/) | Architecture + dated notes |
| [`archive/`](archive/) | Old sketches and core stubs |

## Look without hardware

```bash
cd sim
npm install
npm start
```

Open http://localhost:5173 — hex layout, arms hang down at rest, festival ±30°/±60° pulse at **1×** realtime (`STAEXE_TIME_SCALE=20` to hurry).

## Flash the Mega

```bash
./scripts/verify-mega.sh       # compile + flash/SRAM budgets
./scripts/export-firmware.sh   # dist/STAEXE/ for Arduino IDE
```

Or open `firmware/STAEXE/STAEXE.ino` with `--libraries firmware/lib`. Last check: **~7% flash / ~20% SRAM**.

A4988 microstep jumpers must match `#define MICROSTEPS` (1/8 on the PCB as built).

## Credits

James Delaney — [FAB_LED](https://github.com/sonyhome/FAB_LED) (Dan Truong), [StepperDriver](https://github.com/laurb9/StepperDriver) (Laurentiu Badea, 12-motor patch in-tree), Body & Soul 2022.
