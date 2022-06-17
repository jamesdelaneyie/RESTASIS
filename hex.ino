///*********************************************************/
///********** Stacy *******************************/
///********** Art Installation Body & Soul Festival 2022
///********** by @jamesdelaneyie
///********** IT License 
///*********************************************************/

//********** Libraries & Setup Scripts *************/
#include "arduino.h"

//bool projectOn = false;
//bool isMoving = false;

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
  Serial.begin(9600);
  //MSGEQ7.begin();
  //irrecv.enableIRIn();
  LEDsoff();
}

void setup() {
  delay(2000);
  setupScript();
}

int degrees[] = {30, 60, 90, 120};

bool changed = false;
int changed_count = 0;

unsigned long previousLEDMillis = 0;
const long ledInterval = 10000; 

int direction = 1;

unsigned long previousMotorMillis = 0;
const long motorInterval = 10000; 
int degreesToMove;

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
  //32400000 = 9 hours

  if(currentMillis - previousMotorMillis > motorInterval) {
    previousMotorMillis = currentMillis;
    
    if(direction == 1) {
      int randomColor = random(0,2);
      degreesToMove = degrees[randomColor];
      controllerA.rotate(degreesToMove * -1, 0, degreesToMove * -1, -degreesToMove, degreesToMove, degreesToMove, degreesToMove, degreesToMove * -1, degreesToMove, degreesToMove * -1, 0, degreesToMove);
      direction = 0;
    } else {
      controllerA.rotate(degreesToMove, 0, degreesToMove, degreesToMove, degreesToMove * -1, degreesToMove * -1, degreesToMove * -1, degreesToMove, degreesToMove * -1, degreesToMove, 0, degreesToMove * -1);
      direction = 1;
    }

  }

  if (currentMillis - previousLEDMillis >= ledInterval) {
    previousLEDMillis = currentMillis;
      int randomColor = random(0,11);
      for(int i = 0; i < LEDS_PER_STRIP; i++) {
      if(randomColor == 1) {
          leds[i].r = maxBrightness;
          leds[i].g = maxBrightness;
          leds[i].b = maxBrightness;
      } else if(randomColor == 2) {
          leds[i].r = maxBrightness;
          leds[i].g = 0;
          leds[i].b = 0;
      } else if(randomColor == 3) {
          leds[i].r = 0;
          leds[i].g = maxBrightness;
          leds[i].b = 0;
      } else if(randomColor == 4) {
          leds[i].r = 0;
          leds[i].g = 0;
          leds[i].b = maxBrightness;
      } else if(randomColor == 5) {
          leds[i].r = maxBrightness;
          leds[i].g = maxBrightness;
          leds[i].b = 0;
      } else if(randomColor == 6) {
          leds[i].r = 0;
          leds[i].g = maxBrightness;
          leds[i].b = maxBrightness;
      } else if(randomColor == 7) {
          leds[i].r = maxBrightness;
          leds[i].g = 0;
          leds[i].b = maxBrightness;
      } else if(randomColor == 8) {
          leds[i].r = maxBrightness;
          leds[i].g = maxBrightness / 2;
          leds[i].b = 0;
      } else if(randomColor == 9) {
          leds[i].r = maxBrightness / 2;
          leds[i].g = 0;
          leds[i].b = maxBrightness;
      } else if(randomColor == 10) {
          leds[i].r = maxBrightness;
          leds[i].g = maxBrightness / 2;
          leds[i].b = maxBrightness / 2;
      }
    }
    updateAllStrips(leds);
  }

}