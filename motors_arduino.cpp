/*********************************************************
 * STAEXE — Arduino motors shim (SyncDriver)
 *********************************************************/
#ifndef SIMULATOR

#include "staexe_display.h"
#include "setup_motors.h"

void motors_setup(void) {
  setupMotors();
  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    staexe_motor_degrees[i] = 0.0f;
  }
}

void motors_rotate(
    float d0, float d1, float d2, float d3,
    float d4, float d5, float d6, float d7,
    float d8, float d9, float d10, float d11) {
  const float deltas[STAEXE_MOTOR_COUNT] = {
      d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11};

  for (int i = 0; i < STAEXE_MOTOR_COUNT; i++) {
    staexe_motor_degrees[i] += deltas[i];
  }

  /* Blocking, same as original hex.ino behaviour. */
  controllerA.rotate(
      (double)d0, (double)d1, (double)d2, (double)d3,
      (double)d4, (double)d5, (double)d6, (double)d7,
      (double)d8, (double)d9, (double)d10, (double)d11);
}

void motors_update(unsigned long /*now_ms*/) {
  /* SyncDriver::rotate is blocking on-device; nothing to tick. */
}

#endif /* !SIMULATOR */
