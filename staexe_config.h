/*********************************************************
 * STAEXE — shared configuration (Arduino Mega + simulator)
 *
 * Keep this lean: fixed sizes only, no dynamic allocation.
 * Mega SRAM is ~8KB; the LED frame alone is 70 * 3 = 210 bytes
 * when strips share one buffer (mirrored mode).
 *********************************************************/
#ifndef STAEXE_CONFIG_H
#define STAEXE_CONFIG_H

#define STAEXE_LED_STRIPS 18
#define STAEXE_LEDS_PER_STRIP 70
#define STAEXE_MOTOR_COUNT 12

/*
 * Physical layout (PIXI sim + installation):
 *   6 radial arms × 2 LED strips     = strips 0..11
 *   6 outer hexagon struts           = strips 12..17
 *   12 motors: 2 hubs per arm (inner + outer)
 * Firmware still mirrors one 70-pixel buffer across all strips.
 */

/* Caps brightness on-device; sim mirrors the same value for WYSIWYG */
#ifndef STAEXE_MAX_BRIGHTNESS
#define STAEXE_MAX_BRIGHTNESS 60
#endif

/* Choreography cadence (ms) — LEDs and motors share this pulse */
#ifndef STAEXE_LED_INTERVAL_MS
#define STAEXE_LED_INTERVAL_MS 10000
#endif

#ifndef STAEXE_MOTOR_INTERVAL_MS
#define STAEXE_MOTOR_INTERVAL_MS 10000
#endif

/* Stepper profile used by firmware and for sim timing estimates */
#ifndef STAEXE_MOTOR_RPM
#define STAEXE_MOTOR_RPM 60
#endif

#endif /* STAEXE_CONFIG_H */
