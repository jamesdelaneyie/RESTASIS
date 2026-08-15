# Architecture

STAEXE is a hanging **pointy-top hexagon**: six vertices, **12 steppers in pairs**, LED tubes on motor arms plus six fixed perimeter struts (**18 × 70 WS2812**).

```
STAEXE.ino  →  choreography_loop()
                    │
                    ├─ staexe_leds[] + display_show()
                    └─ motors_rotate(...)
                           │
              ┌────────────┴────────────┐
              ▼                         ▼
      display_arduino.cpp         sim/display_sim.cpp
      motors_arduino.cpp          sim/motors_sim.cpp
      (FAB_LED + SyncDriver)      (NDJSON → PIXI)
```

Choreography never talks to pins. Arduino or the desktop sim implements `staexe_display.h`.

**SRAM:** one 70×3 buffer, `sendPixels` to each strip in turn (~40 ms for all 18 on a 16 MHz Mega). Independent per-strip content = refill that scratch buffer between sends.

**Motors:** `SyncDriver` patched to 12 axes. `rotate()` is still blocking; non-blocking `startRotate` / `nextAction` is how you would interleave LED updates (need ≲15 RPM at 1/8 step for one strip per gap).
