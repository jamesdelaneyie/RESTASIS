/*********************************************************
 * STAEXE simulator — motor shim with timed interpolation
 * so PIXI can show LEDs + steppers moving in unison.
 *********************************************************/
#include "staexe_display.h"
#include <cmath>

static float g_motor_start[STAEXE_MOTOR_COUNT];
static float g_motor_target[STAEXE_MOTOR_COUNT];
static unsigned long g_move_start_ms = 0;
static unsigned long g_move_duration_ms = 0;
static int g_moving = 0;

static unsigned long estimate_move_ms(const float* deltas) {
  float max_deg = 0.0f;
  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    float a = fabsf(deltas[i]);
    if (a > max_deg) max_deg = a;
  }
  /* Rough match to 60 RPM continuous: deg/360 * 60/rpm * 1000 */
  float ms = (max_deg / 360.0f) * (60.0f / (float)STAEXE_MOTOR_RPM) * 1000.0f;
  /* Accel/decel pad so motion is visible beside the 10s LED pulse */
  if (ms < 800.0f) ms = 800.0f;
  if (ms > 4000.0f) ms = 4000.0f;
  return (unsigned long)ms;
}

void motors_setup(void) {
  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    staexe_motor_degrees[i] = 0.0f;
    g_motor_start[i] = 0.0f;
    g_motor_target[i] = 0.0f;
  }
  g_moving = 0;
}

static float g_pending_deltas[STAEXE_MOTOR_COUNT];
static int g_pending_move = 0;

void motors_rotate(
    float d0, float d1, float d2, float d3,
    float d4, float d5, float d6, float d7,
    float d8, float d9, float d10, float d11) {
  const float deltas[STAEXE_MOTOR_COUNT] = {
      d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11};

  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    g_pending_deltas[i] = deltas[i];
    g_motor_start[i] = staexe_motor_degrees[i];
    g_motor_target[i] = staexe_motor_degrees[i] + deltas[i];
  }

  /* Start time is armed on the next motors_update(sim_ms) so motion
   * tracks virtual choreography time, not wall-clock millis(). */
  g_move_duration_ms = estimate_move_ms(deltas);
  g_pending_move = 1;
  g_moving = 0;
}

void motors_update(unsigned long now_ms) {
  if (g_pending_move) {
    g_move_start_ms = now_ms;
    g_pending_move = 0;
    g_moving = 1;
  }

  if (!g_moving) return;

  unsigned long elapsed = now_ms - g_move_start_ms;
  if (elapsed >= g_move_duration_ms) {
    for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
      staexe_motor_degrees[i] = g_motor_target[i];
    }
    g_moving = 0;
    return;
  }

  float t = (float)elapsed / (float)g_move_duration_ms;
  /* Smoothstep for a hint of accel/decel */
  t = t * t * (3.0f - 2.0f * t);
  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    staexe_motor_degrees[i] =
        g_motor_start[i] + (g_motor_target[i] - g_motor_start[i]) * t;
  }
}
