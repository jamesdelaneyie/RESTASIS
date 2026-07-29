/*********************************************************
 * STAEXE — pixel type (no Arduino / FAB_LED dependency)
 *
 * Memory layout matches FAB_LED's grb (g, r, b) so the
 * Arduino display shim can pass this buffer to sendPixels.
 *********************************************************/
#ifndef STAEXE_PIXEL_H
#define STAEXE_PIXEL_H

#include <stdint.h>

typedef struct staexe_pixel_t {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} staexe_pixel_t;

#endif /* STAEXE_PIXEL_H */
