// Restasis / HEX Art Installation for Body & Soul Festival 2022
// by @jamesdelaneyie
// MIT License 


//Include Libraries and setup scripts
#include "arduino.h"
#include "libs/FAB_LED.h"
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "IRremote.h"
#include "pitches.h"

uint64_t projectTimer = 0;


#define LED_STRIPS 18
#define LEDS_PER_STRIP 72

ws2812b<B,4>  strip1;
ws2812b<B,6>  strip2;
ws2812b<B,5>  strip3;

ws2812b<A,2>  strip4;
ws2812b<A,1>  strip5;
ws2812b<A,3>  strip6;

ws2812b<H,4>  strip7;
ws2812b<H,5>  strip8;
ws2812b<H,6>  strip9;

ws2812b<L,1>  strip10;
ws2812b<L,2>  strip11;
ws2812b<L,3>  strip12;

ws2812b<G,2>  strip13;
ws2812b<G,1>  strip14;
ws2812b<D,7>  strip15;

ws2812b<K,5>  strip16;
ws2812b<K,6>  strip17;
ws2812b<K,7>  strip18;

grb leds[LEDS_PER_STRIP];
grb allLEDs[LEDS_PER_STRIP];
char allLEDSColor = ' ';

#include "LEDAnimations.h"
#include "LEDSections.h"

#define MOTOR_STEPS 200
#define RPM 60
#define MOTOR_ACCEL 80
#define MOTOR_DECEL 30
#define MICROSTEPS 8

BasicStepperDriver stepper1(MOTOR_STEPS, 2, 3);
BasicStepperDriver stepper2(MOTOR_STEPS, 4, 5);
BasicStepperDriver stepper3(MOTOR_STEPS, 6, 7);
BasicStepperDriver stepper4(MOTOR_STEPS, 8, 9);
BasicStepperDriver stepper5(MOTOR_STEPS, 10, 11);
BasicStepperDriver stepper6(MOTOR_STEPS, 12, 13);
BasicStepperDriver stepper7(MOTOR_STEPS, 14, 15);
BasicStepperDriver stepper8(MOTOR_STEPS, 16, 17);
BasicStepperDriver stepper9(MOTOR_STEPS, 18, 19);
BasicStepperDriver stepper10(MOTOR_STEPS, 20, 21);
BasicStepperDriver stepper11(MOTOR_STEPS, 22, 23);
BasicStepperDriver stepper12(MOTOR_STEPS, 24, 25);


MultiDriver controller(stepper1, stepper2);
MultiDriver controller2(stepper3, stepper4);
MultiDriver controller3(stepper5, stepper6);
MultiDriver controller4(stepper7, stepper8);
MultiDriver controller5(stepper9, stepper10);
MultiDriver controller6(stepper11, stepper12);


#define PIEZO_PIN 27
#define SOUND_SENSOR_PIN A0
#define LIGHT_SENSOR_PIN A6
#define DISTANCE_SENSOR_PIN A7
#define IR_SENSOR_PIN A8
#define BUTTON_PIN 32

//Define the IR Remote Commands
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

IRrecv irrecv(IR_SENSOR_PIN);
decode_results results;

uint16_t resultCode;
uint16_t lastValue;

long choice; 


// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A4 //Left
#define pinAnalogRight A2 //Right
#define pinReset 34
#define pinStrobe 36
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;



uint8_t maxBrightness = 250;
uint8_t currentMicVolume = 0;
uint8_t currentAmbientBrightness = 0;
uint8_t currentFrontDistance = 0;
char currentIRCommand = ' ';

bool buttonOn = false;
bool projectOn = false;
bool isMoving = false;
bool motorsPowered = false;


void setupMotors() {
  stepper1.begin(RPM, MICROSTEPS);
  stepper1.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper2.begin(RPM, MICROSTEPS);
  stepper2.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper3.begin(RPM, MICROSTEPS);
  stepper3.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  
  stepper4.begin(RPM, MICROSTEPS);
  stepper4.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper5.begin(RPM, MICROSTEPS);
  stepper5.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper6.begin(RPM, MICROSTEPS);
  stepper6.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper7.begin(RPM, MICROSTEPS);
  stepper7.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper8.begin(RPM, MICROSTEPS);
  stepper8.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper9.begin(RPM, MICROSTEPS);
  stepper9.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper10.begin(RPM, MICROSTEPS);
  stepper10.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper11.begin(RPM, MICROSTEPS);
  stepper11.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper12.begin(RPM, MICROSTEPS);
  stepper12.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

}

void setup() {
  delay(3000);
  LEDSoff();
  setupMotors();
  irrecv.enableIRIn();
  MSGEQ7.begin();
}

int startup_sound_melody[] = {
  NOTE_C5, NOTE_G4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_G5
};

int startup_sound_noteDurations[] = {
  4,4,4,8,8,4
};

unsigned long previousMillis = 0;
unsigned long pauseBetweenNotes;
int thisNote;

void playStartupMusic() {
  unsigned long currentMillis = millis();
  if (thisNote < 8 && currentMillis - previousMillis >= pauseBetweenNotes) {
    previousMillis = currentMillis;

    int noteDuration = 1000 / startup_sound_noteDurations[thisNote];
    tone(PIEZO_PIN, startup_sound_melody[thisNote], noteDuration);

    pauseBetweenNotes = noteDuration * 1.30;
    
    thisNote++;
  }
}

unsigned long sensorMillis = 0;

void readSensors(){
  unsigned long currentMillis = millis();
  currentMicVolume = analogRead(SOUND_SENSOR_PIN);
  currentAmbientBrightness = analogRead(LIGHT_SENSOR_PIN);
  currentFrontDistance = analogRead(DISTANCE_SENSOR_PIN);
  currentIRCommand = analogRead(IR_SENSOR_PIN);
  buttonOn = analogRead(BUTTON_PIN);

}

void updateAllStrips() {
  strip1.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip2.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip3.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip4.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip5.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip6.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip7.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip8.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip9.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip10.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip11.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip12.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip13.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip14.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip15.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip16.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip17.sendPixels(LEDS_PER_STRIP, allLEDs);
  strip18.sendPixels(LEDS_PER_STRIP, allLEDs);
}

void allLEDsColor() {
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    if(allLEDSColor == 'white') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = maxBrightness;
      allLEDs[i].b = maxBrightness;
    } else if(allLEDSColor == 'red') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = 0;
      allLEDs[i].b = 0;
    } else if(allLEDSColor == 'green') {
      allLEDs[i].r = 0;
      allLEDs[i].g = maxBrightness;
      allLEDs[i].b = 0;
    } else if(allLEDSColor == 'blue') {
      allLEDs[i].r = 0;
      allLEDs[i].g = 0;
      allLEDs[i].b = maxBrightness;
    } else if(allLEDSColor == 'yellow') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = maxBrightness;
      allLEDs[i].b = 0;
    } else if(allLEDSColor == 'cyan') {
      allLEDs[i].r = 0;
      allLEDs[i].g = maxBrightness;
      allLEDs[i].b = maxBrightness;
    } else if(allLEDSColor == 'magenta') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = 0;
      allLEDs[i].b = maxBrightness;
    } else if(allLEDSColor == 'orange') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = maxBrightness / 2;
      allLEDs[i].b = 0;
    } else if(allLEDSColor == 'purple') {
      allLEDs[i].r = maxBrightness / 2;
      allLEDs[i].g = 0;
      allLEDs[i].b = maxBrightness;
    } else if(allLEDSColor == 'pink') {
      allLEDs[i].r = maxBrightness;
      allLEDs[i].g = maxBrightness / 2;
      allLEDs[i].b = maxBrightness / 2;
    }
  }
  updateAllStrips();
}
  




void HolographicGlitterFizzing() {

}

void CiderSodaAndSemantics() {

}

void BuzzingBreakoutBash() {

}

void SummerSolstice() {

}

void FeastsFeatsAndFeets() {

}

void FullTiltHeads() {

}

void BreathingPilgrimage() {

}





void loop() {

  readSensors();

  if(buttonOn) {

    playStartupMusic();
    
    allLEDsColor();
  }


}