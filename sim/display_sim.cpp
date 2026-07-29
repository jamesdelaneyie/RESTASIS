/*********************************************************
 * STAEXE simulator — display_show / clear (fake LED layer)
 *********************************************************/
#include "staexe_display.h"
#include "sim_export.h"
#include <cstring>

/* Mirrored strips: one logical frame; PIXI expands to 18 rows. */
static uint8_t g_frame_rgb[STAEXE_LEDS_PER_STRIP * 3];
static int g_frame_dirty = 1;

extern "C" const uint8_t* sim_leds_rgb(void) {
  return g_frame_rgb;
}

extern "C" int sim_take_frame_dirty(void) {
  int d = g_frame_dirty;
  g_frame_dirty = 0;
  return d;
}

void display_clear(void) {
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    staexe_leds[i].r = 0;
    staexe_leds[i].g = 0;
    staexe_leds[i].b = 0;
    g_frame_rgb[i * 3 + 0] = 0;
    g_frame_rgb[i * 3 + 1] = 0;
    g_frame_rgb[i * 3 + 2] = 0;
  }
  g_frame_dirty = 1;
}

void display_show(void) {
  for (int i = 0; i < STAEXE_LEDS_PER_STRIP; i++) {
    /* Export as RGB for the renderer (buffer is GRB-ordered struct). */
    g_frame_rgb[i * 3 + 0] = staexe_leds[i].r;
    g_frame_rgb[i * 3 + 1] = staexe_leds[i].g;
    g_frame_rgb[i * 3 + 2] = staexe_leds[i].b;
  }
  g_frame_dirty = 1;
}
