#include "pitches.h"

#define PIEZO_PIN 27

//********** Start Up Melody ***************/
int startup_sound_melody[] = {
  NOTE_C5, NOTE_G4, NOTE_D4, NOTE_G4, NOTE_G4, NOTE_G5
};

int startup_sound_noteDurations[] = {
  8,8,8,16,16,8
};

unsigned long previousStartupMusicMillis = 0;
unsigned long pauseBetweenNotes;
int thisNote;

extern void playStartupMusic() {

  for (int thisNote = 0; thisNote < 6; thisNote++) {

    int noteDuration = 1000 / startup_sound_noteDurations[thisNote];

    tone(PIEZO_PIN, startup_sound_melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    noTone(PIEZO_PIN);

  }

  /*unsigned long currentMillis = millis();

  if (currentMillis - previousStartupMusicMillis >= pauseBetweenNotes) {
    previousStartupMusicMillis = currentMillis;

    int noteDuration = 1000 / startup_sound_noteDurations[thisNote];
    tone(PIEZO_PIN, startup_sound_melody[thisNote], noteDuration);

    pauseBetweenNotes = noteDuration * 1.30;
    
    delay(pauseBetweenNotes);

    noTone(PIEZO_PIN);

    thisNote++;
  }*/
}
//********** END Start Up Melody ***************/