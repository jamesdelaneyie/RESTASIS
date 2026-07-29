/*********************************************************
 * STAEXE — display / motor output shim
 *
 * Choreography writes pixels + issues motor commands here.
 * Arduino: bit-bang WS2812 + SyncDriver steppers.
 * Simulator: update a frame buffer consumed by PIXI.js.
 *********************************************************/
#ifndef STAEXE_DISPLAY_H
#define STAEXE_DISPLAY_H

#include "staexe_config.h"
#include "staexe_pixel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Shared mirrored LED frame (one strip's worth, pushed to all 18). */
extern staexe_pixel_t staexe_leds[STAEXE_LEDS_PER_STRIP];

/* Absolute motor angles in degrees (sim + optional firmware telemetry). */
extern float staexe_motor_degrees[STAEXE_MOTOR_COUNT];

void display_clear(void);
void display_show(void);

void motors_setup(void);

/* Relative rotate — same semantics as SyncDriver::rotate(...). */
void motors_rotate(
    float d0, float d1, float d2, float d3,
    float d4, float d5, float d6, float d7,
    float d8, float d9, float d10, float d11);

/*
 * Advance simulated / non-blocking motion.
 * On Arduino with blocking rotate(), this may be a no-op.
 * Call once per loop with millis().
 */
void motors_update(unsigned long now_ms);

#ifdef __cplusplus
}
#endif

#endif /* STAEXE_DISPLAY_H */
