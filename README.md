# STAEXE

Kinetic LED + stepper installation firmware (Arduino Mega 2560) with a desktop simulator.

## Firmware

Open `hex.ino` in the Arduino IDE (board: **Arduino Mega 2560**). Choreography lives in `staexe_choreography.cpp` and talks only to the display/motor shim (`staexe_display.h`).

## Simulator

Runs the **same C++ choreography** against a fake output layer; PIXI.js renders LEDs + motors together.

```bash
cd sim
make
npm install
npm start
```

Then open http://localhost:5173
