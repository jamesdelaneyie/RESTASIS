#define LEDS_PER_STRIP 72
grb leds[LEDS_PER_STRIP];

extern void cylon() {
  for(int j = 0; j < 4; j++) {
    for (int i = 0; i < LEDS_PER_STRIP; i++) {
        leds[i].r = j*30;
    }
    strip.sendPixels(LEDS_PER_STRIP, leds);
    strip3.sendPixels(LEDS_PER_STRIP, leds);
    strip12.sendPixels(LEDS_PER_STRIP, leds); 
    strip13.sendPixels(LEDS_PER_STRIP, leds);
    strip5.sendPixels(LEDS_PER_STRIP, leds); 
    strip6.sendPixels(LEDS_PER_STRIP, leds); 
    strip15.sendPixels(LEDS_PER_STRIP, leds); 
    strip4.sendPixels(LEDS_PER_STRIP, leds);
    strip18.sendPixels(LEDS_PER_STRIP, leds); 
    strip17.sendPixels(LEDS_PER_STRIP, leds); 
    strip10.sendPixels(LEDS_PER_STRIP, leds);
    strip8.sendPixels(LEDS_PER_STRIP, leds);
    
  }
  for(int j = 4; j > 0; j--) {
    for (int i = 0; i < LEDS_PER_STRIP; i++) {
        leds[i].r = j*30;
    }
    strip.sendPixels(LEDS_PER_STRIP, leds);
    strip3.sendPixels(LEDS_PER_STRIP, leds);
    strip12.sendPixels(LEDS_PER_STRIP, leds); 
    strip13.sendPixels(LEDS_PER_STRIP, leds);
    strip5.sendPixels(LEDS_PER_STRIP, leds); 
    strip6.sendPixels(LEDS_PER_STRIP, leds); 
    strip15.sendPixels(LEDS_PER_STRIP, leds); 
    strip4.sendPixels(LEDS_PER_STRIP, leds);
    strip18.sendPixels(LEDS_PER_STRIP, leds); 
    strip17.sendPixels(LEDS_PER_STRIP, leds); 
    strip10.sendPixels(LEDS_PER_STRIP, leds);
    strip8.sendPixels(LEDS_PER_STRIP, leds);
  }

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
