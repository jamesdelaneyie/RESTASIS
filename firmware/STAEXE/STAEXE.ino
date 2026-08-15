/*********************************************************
 * STAEXE — Art Installation
 * Body & Soul Festival lineage / kinetic LED + stepper piece
 *********************************************************/

#include <Arduino.h>

#include "staexe_display.h"
#include "staexe_choreography.h"

#include "leds_setup.h"
#include "LEDAnimations.h"

#include "sensors.h"
#include "IR_sensor_setup.h"
#include "MSGEQ_setup.h"
#include "startup_melody.h"
#include "routines.h"

void setupScript() {
  motors_setup();
  playStartupMusic();
  Serial.begin(9600);
  choreography_setup();
}

void setup() {
  delay(2000);
  setupScript();
}

void loop() {
  unsigned long currentMillis = millis();
  readSensors(currentMillis);
  choreography_loop(currentMillis);
}
