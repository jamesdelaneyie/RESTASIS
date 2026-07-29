/*********************************************************
 * STAEXE — optional LED helpers (HSV / named colours)
 * Backed by staexe_leds + display_show(); no direct port I/O.
 *********************************************************/
#ifndef LEDS_SETUP_H
#define LEDS_SETUP_H

#include "staexe_display.h"
#include "staexe_config.h"

/* Compatibility aliases used by older sketches / notes */
#define LED_STRIPS STAEXE_LED_STRIPS
#define LEDS_PER_STRIP STAEXE_LEDS_PER_STRIP

static uint8_t maxBrightness = STAEXE_MAX_BRIGHTNESS;

static float hue = 0.0f;
static float color[3];

static float fract(float x) { return x - (int)x; }
static float mix(float a, float b, float t) { return a + (b - a) * t; }

static float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0f, constrain(fabs(fract(h + 1.0f) * 6.0f - 3.0f) - 1.0f, 0.0f, 1.0f), s);
  rgb[1] = b * mix(1.0f, constrain(fabs(fract(h + 0.6666666f) * 6.0f - 3.0f) - 1.0f, 0.0f, 1.0f), s);
  rgb[2] = b * mix(1.0f, constrain(fabs(fract(h + 0.3333333f) * 6.0f - 3.0f) - 1.0f, 0.0f, 1.0f), s);
  return rgb;
}

static void setColor(float* rgb) {
  color[0] = rgb[0];
  color[1] = rgb[1];
  color[2] = rgb[2];
}

static void hueCycle() {
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    setColor(hsv2rgb(hue, 1.0f, 0.25f, color));
    staexe_leds[i].r = (uint8_t)color[0];
    staexe_leds[i].g = (uint8_t)color[1];
    staexe_leds[i].b = (uint8_t)color[2];
    hue += 0.01f;
    if (hue > 1.0f) hue -= 1.0f;
  }
}

static void updateAllStrips() {
  display_show();
}

static void ledsSimpleColor(const char* name) {
  const uint8_t m = maxBrightness;
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    if (name[0] == 'w') { /* white */
      staexe_leds[i].r = m; staexe_leds[i].g = m; staexe_leds[i].b = m;
    } else if (name[0] == 'r') {
      staexe_leds[i].r = m; staexe_leds[i].g = 0; staexe_leds[i].b = 0;
    } else if (name[0] == 'g') {
      staexe_leds[i].r = 0; staexe_leds[i].g = m; staexe_leds[i].b = 0;
    } else if (name[0] == 'b' && name[2] == 'u') {
      staexe_leds[i].r = 0; staexe_leds[i].g = 0; staexe_leds[i].b = m;
    } else if (name[0] == 'y') {
      staexe_leds[i].r = m; staexe_leds[i].g = m; staexe_leds[i].b = 0;
    } else if (name[0] == 'c') {
      staexe_leds[i].r = 0; staexe_leds[i].g = m; staexe_leds[i].b = m;
    } else if (name[0] == 'm') {
      staexe_leds[i].r = m; staexe_leds[i].g = 0; staexe_leds[i].b = m;
    } else if (name[0] == 'o') {
      staexe_leds[i].r = m; staexe_leds[i].g = (uint8_t)(m / 2); staexe_leds[i].b = 0;
    } else if (name[0] == 'p' && name[1] == 'u') {
      staexe_leds[i].r = (uint8_t)(m / 2); staexe_leds[i].g = 0; staexe_leds[i].b = m;
    } else if (name[0] == 'p') { /* pink */
      staexe_leds[i].r = m; staexe_leds[i].g = (uint8_t)(m / 2); staexe_leds[i].b = (uint8_t)(m / 2);
    }
  }
  display_show();
}

#endif /* LEDS_SETUP_H */
