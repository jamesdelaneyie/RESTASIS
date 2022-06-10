extern void topLeftTri(grb leds[LEDS_PER_STRIP]) {
    strip1.sendPixels(LEDS_PER_STRIP, leds);
    strip2.sendPixels(LEDS_PER_STRIP, leds);
    strip3.sendPixels(LEDS_PER_STRIP, leds);
}