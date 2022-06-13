#include "FAB_LED.h"

//********** LEDs Setup *************/
#define LED_STRIPS 18
#define LEDS_PER_STRIP 72

ws2812b<H,4>  strip1;
ws2812b<H,5>  strip2;
ws2812b<H,6>  strip3;


ws2812b<B,4>  strip4;
ws2812b<B,5>  strip5;
ws2812b<B,6>  strip6;

/*
ws2812b<A,2>  strip4;
ws2812b<A,1>  strip5;
ws2812b<A,3>  strip6;



ws2812b<L,1>  strip10;
ws2812b<L,2>  strip11;
ws2812b<L,3>  strip12;

ws2812b<G,2>  strip13;
ws2812b<G,1>  strip14;
ws2812b<D,7>  strip15;

ws2812b<K,5>  strip16;
ws2812b<K,6>  strip17;
ws2812b<K,7>  strip18;
*/

grb leds[LEDS_PER_STRIP];
//grb leds[LEDS_PER_STRIP];

/*
grb leds1[LEDS_PER_STRIP];
grb leds2[LEDS_PER_STRIP];
grb leds3[LEDS_PER_STRIP];*/
/*grb leds4[LEDS_PER_STRIP];
grb leds5[LEDS_PER_STRIP];
grb leds6[LEDS_PER_STRIP];
grb leds7[LEDS_PER_STRIP];
grb leds8[LEDS_PER_STRIP];
grb leds9[LEDS_PER_STRIP];
grb leds10[LEDS_PER_STRIP];
grb leds11[LEDS_PER_STRIP];
grb leds12[LEDS_PER_STRIP];
grb leds13[LEDS_PER_STRIP];
grb leds14[LEDS_PER_STRIP];
grb leds15[LEDS_PER_STRIP];
grb leds16[LEDS_PER_STRIP];
grb leds17[LEDS_PER_STRIP];
grb leds18[LEDS_PER_STRIP];

grb topRightLeds[LEDS_PER_STRIP];
grb topLeftLeds[LEDS_PER_STRIP];
grb centerRightLeds[LEDS_PER_STRIP];
grb centerLeftLeds[LEDS_PER_STRIP];
grb bottomRightLeds[LEDS_PER_STRIP];
grb bottomLeftLeds[LEDS_PER_STRIP];

grb outsideLEDs[LEDS_PER_STRIP];
grb insideLEDs[LEDS_PER_STRIP];
*/



//#include "LEDAnimations.h"

/*void updateTopLeftLeds(grb ledArray[]) {
    strip1.sendPixels(LEDS_PER_STRIP, ledArray);
    strip2.sendPixels(LEDS_PER_STRIP, ledArray);
    strip3.sendPixels(LEDS_PER_STRIP, ledArray);
}

void updateTopRightLeds(grb ledArray[]) {
    strip4.sendPixels(LEDS_PER_STRIP, ledArray);
    strip5.sendPixels(LEDS_PER_STRIP, ledArray);
    strip6.sendPixels(LEDS_PER_STRIP, ledArray);
}

void updateCenterLeftLeds(grb ledArray[]) {
    strip7.sendPixels(LEDS_PER_STRIP, ledArray);
    strip8.sendPixels(LEDS_PER_STRIP, ledArray);
    strip9.sendPixels(LEDS_PER_STRIP, ledArray);
}

void updateCenterRightLeds(grb ledArray[]) {
    strip10.sendPixels(LEDS_PER_STRIP, ledArray);
    strip11.sendPixels(LEDS_PER_STRIP, ledArray);
    strip12.sendPixels(LEDS_PER_STRIP, ledArray);
}

void updateBottomLeftLeds(grb ledArray[]) {
    strip13.sendPixels(LEDS_PER_STRIP, ledArray);
    strip14.sendPixels(LEDS_PER_STRIP, ledArray);
    strip15.sendPixels(LEDS_PER_STRIP, ledArray);
}

void updateBottomRightLeds(grb ledArray[]) {
    strip16.sendPixels(LEDS_PER_STRIP, ledArray);
    strip17.sendPixels(LEDS_PER_STRIP, ledArray);
    strip18.sendPixels(LEDS_PER_STRIP, ledArray);
}

*/

uint8_t maxBrightness = 63;

float hue = 0.0;

float color[3];

float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}


void updateAllStrips(grb allLEDS[]) {
  strip1.sendPixels(LEDS_PER_STRIP, leds);
  strip2.sendPixels(LEDS_PER_STRIP, leds);
  strip3.sendPixels(LEDS_PER_STRIP, leds);
  strip4.sendPixels(LEDS_PER_STRIP, leds);
  strip5.sendPixels(LEDS_PER_STRIP, leds);
  strip6.sendPixels(LEDS_PER_STRIP, leds);
  /*strip7.sendPixels(LEDS_PER_STRIP, leds);
  strip8.sendPixels(LEDS_PER_STRIP, leds);
  strip9.sendPixels(LEDS_PER_STRIP, leds);
  strip10.sendPixels(LEDS_PER_STRIP, leds);
  strip11.sendPixels(LEDS_PER_STRIP, leds);
  strip12.sendPixels(LEDS_PER_STRIP, leds);
  strip13.sendPixels(LEDS_PER_STRIP, leds);
  strip14.sendPixels(LEDS_PER_STRIP, leds);
  strip15.sendPixels(LEDS_PER_STRIP, leds);
  strip16.sendPixels(LEDS_PER_STRIP, leds);
  strip17.sendPixels(LEDS_PER_STRIP, leds);
  strip18.sendPixels(LEDS_PER_STRIP, leds);*/
}



void setColor(float *rgb){
  color[0] = rgb[0];
  color[1] = rgb[1];
  color[2] = rgb[2];
}

void ledsColor(float hue, float saturation, float brightness) {
  setColor(hsv2rgb(hue, saturation, brightness, color));
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    leds[i].r = color[0];
    leds[i].g = color[1];
    leds[i].b = color[2];
  }
  updateAllStrips(leds);
}

void ledsSimpleColor(char color) {
  for (int i = 0; i < LEDS_PER_STRIP; i++) {
    if(color == 'white') {
      leds[i].r = maxBrightness;
      leds[i].g = maxBrightness;
      leds[i].b = maxBrightness;
    } else if(color == 'red') {
      leds[i].r = maxBrightness;
      leds[i].g = 0;
      leds[i].b = 0;
    } else if(color == 'green') {
      leds[i].r = 0;
      leds[i].g = maxBrightness;
      leds[i].b = 0;
    } else if(color == 'blue') {
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = maxBrightness;
    } else if(color == 'yellow') {
      leds[i].r = maxBrightness;
      leds[i].g = maxBrightness;
      leds[i].b = 0;
    } else if(color == 'cyan') {
      leds[i].r = 0;
      leds[i].g = maxBrightness;
      leds[i].b = maxBrightness;
    } else if(color == 'magenta') {
      leds[i].r = maxBrightness;
      leds[i].g = 0;
      leds[i].b = maxBrightness;
    } else if(color == 'orange') {
      leds[i].r = maxBrightness;
      leds[i].g = maxBrightness / 2;
      leds[i].b = 0;
    } else if(color == 'purple') {
      leds[i].r = maxBrightness / 2;
      leds[i].g = 0;
      leds[i].b = maxBrightness;
    } else if(color == 'pink') {
      leds[i].r = maxBrightness;
      leds[i].g = maxBrightness / 2;
      leds[i].b = maxBrightness / 2;
    }
  }
  updateAllStrips(leds);
}

