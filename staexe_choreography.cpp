/*********************************************************
 * STAEXE — LED + motor choreography
 *
 * Pure-ish logic: only touches staexe_leds + display/motor shims.
 * Safe for Mega: fixed arrays, no heap, no STL.
 *********************************************************/
#include "staexe_choreography.h"
#include "staexe_display.h"
#include "staexe_config.h"

#ifndef SIMULATOR
#include <Arduino.h>
#else
#include "arduino_shim.h"
#endif

static unsigned long previous_led_ms = 0;
static unsigned long previous_motor_ms = 0;
static int motor_direction = 1;
static int degrees_to_move = 30;

static const int k_move_choices[] = {30, 60, 90, 120};

static void fill_solid(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    staexe_leds[i].r = r;
    staexe_leds[i].g = g;
    staexe_leds[i].b = b;
  }
}

static void apply_palette(int index) {
  const uint8_t m = (uint8_t)STAEXE_MAX_BRIGHTNESS;
  switch (index) {
    case 1:  fill_solid(m, m, m); break;           /* white */
    case 2:  fill_solid(m, 0, 0); break;           /* red */
    case 3:  fill_solid(0, m, 0); break;           /* green */
    case 4:  fill_solid(0, 0, m); break;           /* blue */
    case 5:  fill_solid(m, m, 0); break;           /* yellow */
    case 6:  fill_solid(0, m, m); break;           /* cyan */
    case 7:  fill_solid(m, 0, m); break;           /* magenta */
    case 8:  fill_solid(m, (uint8_t)(m / 2), 0); break; /* orange */
    case 9:  fill_solid((uint8_t)(m / 2), 0, m); break; /* purple */
    case 10: fill_solid(m, (uint8_t)(m / 2), (uint8_t)(m / 2)); break; /* pink */
    default: fill_solid(0, 0, 0); break;
  }
}

void choreography_setup(void) {
  previous_led_ms = 0;
  previous_motor_ms = 0;
  motor_direction = 1;
  degrees_to_move = 30;
  display_clear();
}

void choreography_loop(unsigned long now_ms) {
  motors_update(now_ms);

  if (now_ms - previous_motor_ms > (unsigned long)STAEXE_MOTOR_INTERVAL_MS) {
    previous_motor_ms = now_ms;

    if (motor_direction == 1) {
      int choice = (int)random(4);
      degrees_to_move = k_move_choices[choice];
      const float d = (float)degrees_to_move;
      motors_rotate(
          -d, 0, -d, -d,
           d,  d,  d, -d,
           d, -d,  0,  d);
      motor_direction = 0;
    } else {
      const float d = (float)degrees_to_move;
      motors_rotate(
           d, 0,  d,  d,
          -d, -d, -d,  d,
          -d,  d,  0, -d);
      motor_direction = 1;
    }
  }

  if (now_ms - previous_led_ms >= (unsigned long)STAEXE_LED_INTERVAL_MS) {
    previous_led_ms = now_ms;
    apply_palette((int)random(11));
    display_show();
  }
}
