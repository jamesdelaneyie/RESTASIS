/*********************************************************
 * STAEXE — legacy animation helpers (shim-backed)
 *********************************************************/
#ifndef LED_ANIMATIONS_H
#define LED_ANIMATIONS_H

#include "staexe_display.h"

inline void cylon() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
      staexe_leds[i].r = (uint8_t)(j * 30);
      staexe_leds[i].g = 0;
      staexe_leds[i].b = 0;
    }
    display_show();
  }
  for (int j = 4; j > 0; j--) {
    for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
      staexe_leds[i].r = (uint8_t)(j * 30);
      staexe_leds[i].g = 0;
      staexe_leds[i].b = 0;
    }
    display_show();
  }
  display_clear();
}

inline void LEDsoff() {
  display_clear();
}

#endif /* LED_ANIMATIONS_H */
