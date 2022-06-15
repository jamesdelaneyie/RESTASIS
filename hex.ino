///*********************************************************/
///********** Restasis / HEX *******************************/
///********** Art Installation Body & Soul Festival 2022
///********** by @jamesdelaneyie
///********** IT License 
///*********************************************************/

//********** Libraries & Setup Scripts *************/
#include "arduino.h"

bool projectOn = false;
bool isMoving = false;

#include "leds_setup.h"

#include "LEDAnimations.h"


#include "setup_motors.h"

#include "sensors.h"
#include "IR_sensor_setup.h"
#include "MSGEQ_setup.h"

#include "startup_melody.h"

#include "routines.h"


void setupScript() {
  setupMotors();
  playStartupMusic();
  MSGEQ7.begin();
  irrecv.enableIRIn();
  LEDsoff();
}

void setup() {
  delay(2000);
  setupScript();
}

bool changed = false;
int changed_count = 0;

void fadeToBlackBy(int numberOfLEDs, int fadeSpeed) {
  for(int i = 0; i < numberOfLEDs; i++) {
    int8_t rValue = leds[i].r - 10;
    leds[i].r = rValue;
    int8_t gValue = leds[i].g - 10;
    leds[i].g = gValue;
    int8_t bValue = leds[i].b - 10;
    leds[i].b = bValue;
  }
  updateAllStrips(leds);
}

void glitter(grb leds[], int numberOfLEDs) {
  for(int i = 0; i < numberOfLEDs; i++) {
    leds[i].r = 0;
    leds[i].g = 0;
    leds[i].b = 0;
  }
  int LEDToLight = random(numberOfLEDs);
  leds[LEDToLight].r = maxBrightness;
  leds[LEDToLight].g = maxBrightness;
  leds[LEDToLight].b = maxBrightness;
  updateAllStrips(leds);
}

void loop() {

  unsigned long currentMillis = millis();
  readSensors(currentMillis);

  //180000 = 3 mins 
  //600000 = 10 mins
  //3600000 = 1 hour
  //7200000 = 2 hours
  //10800000 = 3 hours
  //14400000 = 4 hours
  //18000000 = 5 hours
  //21600000 = 6 hours
  //25200000 = 7 hours
  //28800000 = 8 hours

  if(changed == false) { 
    for(int j = 0; j < LEDS_PER_STRIP; j++) {
      leds[j].r = maxBrightness;
      leds[j].g = maxBrightness;
      leds[j].b = maxBrightness;
    }
    updateAllStrips(leds);
    changed = true;
  }

  controllerA.rotate(-60, 60, -60, 60, -60, 60, -60, 60, -60, 60, -60, 60);

  delay(6000);

  controllerA.rotate(60, -60, 60, -60, 60, -60, 60, -60, 60, -60, 60, -60);

  delay(6000);

  controllerA.rotate(-60, 60, -60, 60, -60, 60, -60, 60, -60, 60, -60, 60);

  delay(6000);

  controllerA.rotate(60, -60, 60, -60, 60, -60, 60, -60, 60, -60, 60, -60);

  delay(2000);

  //fadeToBlackBy(LEDS_PER_STRIP, 1);
  //delay(2000);

}