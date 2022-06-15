
#include "IRremote.h"
//********** Define the IR Remote Commands
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

IRrecv irrecv(IR_SENSOR_PIN);
decode_results results;

uint16_t resultCode;
uint16_t lastValue;

long choice; 