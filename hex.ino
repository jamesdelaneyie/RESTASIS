////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Fast Adressable Bitbang LED Library
/// Copyright (c)2015, 2017 Dan Truong
///
/// This is the simplest exmple to use the library.
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "libs/FAB_LED.h"
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "IRremote.h"




// Declare the LED protocol and the port
ws2812b<B,4>  strip;
ws2812b<B,6>  strip2;
ws2812b<B,5>  strip3;

ws2812b<A,2>  strip5;
ws2812b<A,1>  strip6;
ws2812b<A,3>  strip7;

ws2812b<H,4>  strip8;
ws2812b<H,5>  strip9;
ws2812b<H,6>  strip10;

ws2812b<L,1>  strip11;
ws2812b<L,2>  strip12;
ws2812b<L,3>  strip13;

ws2812b<G,2>  strip14;
ws2812b<G,1>  strip15;
ws2812b<D,7>  strip4;

ws2812b<K,5>  strip16;
ws2812b<K,6>  strip17;
ws2812b<K,7>  strip18;

#include "cylon.h"



// How bright the LEDs will be (max 255)
const uint8_t maxBrightness = 250;


#define LED_STRIPS 18
//#define LEDS_PER_STRIP 72

#define MOTOR_STEPS 200
#define RPM 60
#define MOTOR_ACCEL 80
#define MOTOR_DECEL 30

#define MICROSTEPS 8

bool isMoving = false;
bool forwards = true;
unsigned long currentTime;
unsigned long previousTime;
long rotationAmount; 
long rotationAmount2;
int degreeAmounts[] = {15, 30, 45, 60};//90, 120, 135};
int degreeAmountsInner[] = {15, 30, 45, 60, 90, 120, 160};

long randomNumber;
long randomNumber2;

BasicStepperDriver stepper9(MOTOR_STEPS, 2, 3);
BasicStepperDriver stepper10(MOTOR_STEPS, 4, 5);

MultiDriver controller2(stepper9, stepper10);

//grb leds[LEDS_PER_STRIP];

uint8_t greenTimer = 0;
int sensorValue = 0;
int outputValue = 0;

////////////////////////////////////////////////////////////////////////////////
// This method is automatically called once when the board boots.
////////////////////////////////////////////////////////////////////////////////
void setup()
{
  // Turn off the LEDs
    //analogReference(EXTERNAL);
    Serial.begin(9600);
    pinMode(32, INPUT); 

    stepper9.begin(RPM, MICROSTEPS);
    stepper9.setSpeedProfile(stepper9.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
    
    stepper10.begin(RPM, MICROSTEPS);
    stepper10.setSpeedProfile(stepper9.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

    stepper9.rotate(-30);
    stepper10.rotate(-93);


  strip.clear(LEDS_PER_STRIP);
  strip2.clear(LEDS_PER_STRIP);
  strip3.clear(LEDS_PER_STRIP);
  strip4.clear(LEDS_PER_STRIP);
  strip5.clear(LEDS_PER_STRIP);
  strip6.clear(LEDS_PER_STRIP);
  strip7.clear(LEDS_PER_STRIP);
  strip8.clear(LEDS_PER_STRIP);
  strip9.clear(LEDS_PER_STRIP);
  strip10.clear(LEDS_PER_STRIP);
  strip11.clear(LEDS_PER_STRIP);
  strip12.clear(LEDS_PER_STRIP);
  strip13.clear(LEDS_PER_STRIP);
  strip14.clear(LEDS_PER_STRIP);
  strip15.clear(LEDS_PER_STRIP);
  strip16.clear(LEDS_PER_STRIP);
  strip17.clear(LEDS_PER_STRIP);
  strip18.clear(LEDS_PER_STRIP);
}


//Assign the port/pins to the explicit LED outputs on the PCB

//Light up HEX outside -> Inside ✓
//Fade colors in out ✓
    // Entire HEX ✓
    // Per Triangle 


//Recreate original light pattern from mockup from 2018
//Color wipe entire HEX 
//Light up HEX sequentially from bottom to top

void topLeftTri(grb leds[LEDS_PER_STRIP]) {
    strip.sendPixels(LEDS_PER_STRIP, leds);
    strip2.sendPixels(LEDS_PER_STRIP, leds);
    strip3.sendPixels(LEDS_PER_STRIP, leds);
}



// ========================== cylon ==========================


void update_non_blocking(uint8_t outputValue) {
  static uint8_t wait = 1;
  static unsigned long lastUpdate = 0;
  static uint16_t currentLed = 0;
 
  unsigned long now = micros();

  if (now > lastUpdate + wait) {
    
    leds[currentLed].r = outputValue;
    leds[currentLed+1].r = outputValue;
    leds[currentLed+2].r = outputValue;
    leds[currentLed+3].r = outputValue;

    strip.sendPixels(LEDS_PER_STRIP, leds); 
    strip3.sendPixels(LEDS_PER_STRIP, leds);
    strip12.sendPixels(LEDS_PER_STRIP, leds);
    strip13.sendPixels(LEDS_PER_STRIP, leds);
    currentLed = currentLed > LEDS_PER_STRIP ? 0 : currentLed+4;
    
    lastUpdate = now;
  }
}

float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

float RawToLux(int raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}























//Button on back to turn on/off the installation
//IR Remote Control button to turn on/off the installation (backup)
//IR Remote Control button to control startup routine option (backup)
//Ambient light sensor to control the intensity of the animations / time of day
//Audio sensor to control the intensity of the animations / reactions
//Distance sensor to control the level the arms open up to
//IR Remote Control to set internal time keeping (+/- 1 hour) (if power goes down)
//IR Remote Control to turn on music reactive mode (alt version for MSGEQ direct line in)
//IR Remote Control to turn on/off custom modes
//IR Remote Control to set all arms to centre and turn off LEDS 

//Loop
//1. Read ambient light sensor
//2. Read audio sensor
//3. Read distance sensor
//4. Read IR Remote Control
//5. Read Button
//6. Construct Audio Signals
//6. Update LEDs
//7. Update Arms




void loop() { 


    //unsigned wait_time = controller.nextAction();
    unsigned wait_time2 = controller2.nextAction();

    currentTime = millis();

    long emptyValue = 0;
    randomNumber = random(0,3);

 
 if(isMoving == false && wait_time2 == 0) {
     
      if(forwards == true) {
        long randomNumber = random(0,6);
        rotationAmount = degreeAmountsInner[randomNumber];
        long randomNumber2 = random(0,9);
        rotationAmount2 = degreeAmounts[randomNumber2];
      }
      if(forwards == true) {
        controller2.startRotate(rotationAmount, rotationAmount2, rotationAmount2 * -1, rotationAmount * -1);
      } else {
        controller2.startRotate(rotationAmount * -1, rotationAmount2 * -1, rotationAmount2, rotationAmount);
      }
      isMoving = true;
      forwards = !forwards;
    }
        /*cylon();

        cylon();

        cylon();*/

        int val = digitalRead(32);
        //digitalWrite(ledPin, val);
       // Serial.println(val); 
        if(val == HIGH) {
          cylon();
        } else {
          strip.clear(LEDS_PER_STRIP);
          strip2.clear(LEDS_PER_STRIP);
          strip3.clear(LEDS_PER_STRIP);
          strip4.clear(LEDS_PER_STRIP);
          strip5.clear(LEDS_PER_STRIP);
          strip6.clear(LEDS_PER_STRIP);
          strip7.clear(LEDS_PER_STRIP);
          strip8.clear(LEDS_PER_STRIP);
          strip9.clear(LEDS_PER_STRIP);
          strip10.clear(LEDS_PER_STRIP);
        }
        int sensorValue = analogRead(A6);
        Serial.println(sensorValue);
        outputValue = map(sensorValue, 600, 700, 0, 200);
        update_non_blocking(outputValue);

       // Serial.println(outputValue);
       // Serial.println(sensorValue); 

       // delay(100);

      // int rawValue = analogRead(A6);
     //  Serial.println(RawToLux(rawValue)); 
        //Serial.println(sensorValueTwo); 
        /*for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            
            leds[j--].r = 50;
            leds[j].r = 50;
            leds[j++].r = 50;
            
            //stripe11.sendPixels(LEDS_PER_STRIP, leds);
            strip.sendPixels(LEDS_PER_STRIP, leds); 
            strip3.sendPixels(LEDS_PER_STRIP, leds);
           
        }
        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].r = 0;
        }

        for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            
            leds[j--].g = 50;
            leds[j].g = 50;
            leds[j++].g = 50;
            
            //stripe11.sendPixels(LEDS_PER_STRIP, leds);
            strip12.sendPixels(LEDS_PER_STRIP, leds); 
            strip13.sendPixels(LEDS_PER_STRIP, leds);
           
        }
        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].g = 0;
        }
        
        
        
        for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            
            leds[j--].b = 50;
            leds[j++].b = 50;
            strip5.sendPixels(LEDS_PER_STRIP, leds); 
            strip6.sendPixels(LEDS_PER_STRIP, leds); 
           
        }
        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].b = 0;
        }



        for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            leds[j--].g = 50;
            leds[j].g = 50;
            leds[j++].g = 50;
            //strip16.sendPixels(LEDS_PER_STRIP, leds); 
            strip15.sendPixels(LEDS_PER_STRIP, leds); 
            strip4.sendPixels(LEDS_PER_STRIP, leds);
            //strip13.sendPixels(LEDS_PER_STRIP, leds);
        }
        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].g = 0;
        }


        for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            leds[j--].b = 50;
            leds[j].b = 50;
            leds[j++].b = 50;
            strip18.sendPixels(LEDS_PER_STRIP, leds); 
            strip17.sendPixels(LEDS_PER_STRIP, leds); 
        }
        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].b = 0;
        }



        for(int j = 0; j < LEDS_PER_STRIP; j=j+2) {
            
            leds[j--].r = 50;
            leds[j].r = 50;
            leds[j++].r = 50;
            
            strip10.sendPixels(LEDS_PER_STRIP, leds);
            //strip9.sendPixels(LEDS_PER_STRIP, leds);
            strip8.sendPixels(LEDS_PER_STRIP, leds);
           
        }

        for(int j = 0; j < LEDS_PER_STRIP; j++) {
            leds[j].r = 0;
        }*/





        
       


        

}