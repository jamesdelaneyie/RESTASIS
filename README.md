# STAEXE

Kinetic LED + stepper installation firmware (Arduino Mega 2560) with a desktop simulator.

## Where things stand

- **Firmware** — choreography in lean C++ (`staexe_choreography.cpp`) via a display/motor shim; Mega backends push WS2812 + SyncDriver steppers.
- **Simulator** — same C++ loop streams frames to a PIXI viewer laid out as a **hexagon**: **12 motors in pairs at the vertices**, each holding an LED strip aimed at the centre (strips rotate with the motor arms), plus **6 fixed strut** strips on the outer edges.
- **Mega check** — `scripts/verify-mega.sh` compiles for Mega 2560 and fails if flash/SRAM budgets are exceeded. `scripts/export-firmware.sh` packages a flash-ready sketch under `dist/STAEXE/`.

## Firmware (Arduino Mega 2560)

```bash
# Compile + memory report (needs arduino-cli + arduino:avr core)
./scripts/verify-mega.sh

# Package uploadable sketch + zip
./scripts/export-firmware.sh
# → dist/STAEXE/STAEXE.ino  (open this folder in Arduino IDE)
# → dist/STAEXE-firmware.zip
# → dist/memory-report.txt
```

Or open `hex.ino` at the repo root in the Arduino IDE (board: **Arduino Mega 2560**). CI runs the same Mega verify on every push (`.github/workflows/mega-verify.yml`) and uploads the export artifact.

## Simulator

```bash
cd sim
npm install
npm start
```

Then open http://localhost:5173

On macOS, if `make` fails with missing headers:

```bash
xcode-select --install
export SDKROOT="$(xcrun --sdk macosx --show-sdk-path)"
cd sim && make clean && make
```

Optional: `STAEXE_TIME_SCALE=20` compresses the 10s LED/motor pulse for faster iteration. Default is **1** (realtime, same as Mega).

## Cloud agent environment

`.cursor/environment.json` installs Node + Arduino CLI, builds the sim, and serves it on port **5173** for review while an agent runs.
