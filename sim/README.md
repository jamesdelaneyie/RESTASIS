# Simulator

Desktop preview of the same C++ choreography that runs on the Mega. A small binary emits LED + motor frames; a Node server fans them to a PIXI page.

```bash
npm install
npm start
```

Open http://localhost:5173. Default is **1× realtime**. `STAEXE_TIME_SCALE=20 npm start` speeds the 10 s pulse.

The Makefile compiles `staexe_choreography.cpp` / `staexe_state.cpp` from `../firmware/STAEXE` with fake I/O (`display_sim.cpp`, `motors_sim.cpp`). It never links FAB_LED or StepperDriver.
