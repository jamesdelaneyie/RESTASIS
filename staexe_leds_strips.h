/*********************************************************
 * STAEXE — Arduino LED strip pin map (FAB_LED / Mega)
 *********************************************************/
#ifndef STAEXE_LEDS_STRIPS_H
#define STAEXE_LEDS_STRIPS_H

#ifndef SIMULATOR

#include "FAB_LED.h"

ws2812b<G,5>  strip1;
ws2812b<E,4>  strip2;
ws2812b<E,5>  strip3;
ws2812b<B,4>  strip4;
ws2812b<B,5>  strip5;
ws2812b<B,6>  strip6;
ws2812b<D,1>  strip7;
ws2812b<D,2>  strip8;
ws2812b<D,3>  strip9;
ws2812b<L,1>  strip10;
ws2812b<L,2>  strip11;
ws2812b<L,3>  strip12;
ws2812b<D,7>  strip13;
ws2812b<G,1>  strip14;
ws2812b<G,2>  strip15;
ws2812b<K,5>  strip16;
ws2812b<K,6>  strip17;
ws2812b<K,7>  strip18;

#endif /* !SIMULATOR */

#endif /* STAEXE_LEDS_STRIPS_H */
