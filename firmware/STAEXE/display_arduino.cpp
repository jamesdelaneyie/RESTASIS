/*********************************************************
 * STAEXE — Arduino display_show / display_clear shim
 *********************************************************/
#ifndef SIMULATOR

#include "staexe_display.h"
#include "staexe_leds_strips.h"
#include "FAB_LED.h"

/* FAB_LED grb layout matches staexe_pixel_t (g,r,b). */
static grb* as_grb(staexe_pixel_t* p) {
  return reinterpret_cast<grb*>(p);
}

void display_clear(void) {
  strip1.clear(STAEXE_LEDS_PER_STRIP);
  strip2.clear(STAEXE_LEDS_PER_STRIP);
  strip3.clear(STAEXE_LEDS_PER_STRIP);
  strip4.clear(STAEXE_LEDS_PER_STRIP);
  strip5.clear(STAEXE_LEDS_PER_STRIP);
  strip6.clear(STAEXE_LEDS_PER_STRIP);
  strip7.clear(STAEXE_LEDS_PER_STRIP);
  strip8.clear(STAEXE_LEDS_PER_STRIP);
  strip9.clear(STAEXE_LEDS_PER_STRIP);
  strip10.clear(STAEXE_LEDS_PER_STRIP);
  strip11.clear(STAEXE_LEDS_PER_STRIP);
  strip12.clear(STAEXE_LEDS_PER_STRIP);
  strip13.clear(STAEXE_LEDS_PER_STRIP);
  strip14.clear(STAEXE_LEDS_PER_STRIP);
  strip15.clear(STAEXE_LEDS_PER_STRIP);
  strip16.clear(STAEXE_LEDS_PER_STRIP);
  strip17.clear(STAEXE_LEDS_PER_STRIP);
  strip18.clear(STAEXE_LEDS_PER_STRIP);

  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    staexe_leds[i].r = 0;
    staexe_leds[i].g = 0;
    staexe_leds[i].b = 0;
  }
}

void display_show(void) {
  grb* pixels = as_grb(staexe_leds);
  strip1.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip2.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip3.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip4.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip5.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip6.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip7.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip8.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip9.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip10.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip11.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip12.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip13.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip14.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip15.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip16.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip17.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
  strip18.sendPixels(STAEXE_LEDS_PER_STRIP, pixels);
}

#endif /* !SIMULATOR */
