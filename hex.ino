///*********************************************************/
///********** Restasis / HEX *******************************/
///********** Art Installation Body & Soul Festival 2022
///********** by @jamesdelaneyie
///********** IT License 
///*********************************************************/

//********** Libraries & Setup Scripts *************/
#include "arduino.h"


uint64_t projectTimer = 0;
bool projectOn = false;
bool isMoving = false;


#include "leds_setup.h"
#include "setup_motors.h"


#include "IR_sensor_setup.h"
#include "MSGEQ_setup.h"


#include "startup_melody.h"
#include "sensors.h"
#include "routines.h"

void setup() {
  delay(3000);
  //setupMotors();
  playStartupMusic();
  //delay(3000);
  //irrecv.enableIRIn();
  //MSGEQ7.begin();
  //Serial.begin(9600);
  //LEDSoff();
}





void loop() {

  unsigned long currentMillis = millis();
  readSensors(currentMillis);

  for(int j = 0; j < LEDS_PER_STRIP; j++) {
            
        leds[j].r = 50;
        leds[j].g = 0;
        leds[j].b = 0;
        
        strip1.sendPixels(LEDS_PER_STRIP, leds);
        //strip4.sendPixels(LEDS_PER_STRIP, leds);
        
    }

    for(int j = 0; j < LEDS_PER_STRIP; j++) {
            
        leds[j].r = 0;
        leds[j].g = 50;
        leds[j].b = 0;
        
        strip2.sendPixels(LEDS_PER_STRIP, leds);
        //strip5.sendPixels(LEDS_PER_STRIP, leds);
        
    }

    for(int j = 0; j < LEDS_PER_STRIP; j++) {
            
        leds[j].r = 0;
        leds[j].g = 0;
        leds[j].b = 50;
        
        strip3.sendPixels(LEDS_PER_STRIP, leds);
        //strip6.sendPixels(LEDS_PER_STRIP, leds);
        
    }



}