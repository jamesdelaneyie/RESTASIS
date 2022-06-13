//********** START Sensor Reading  ***************/
#define SOUND_SENSOR_PIN A0
#define LIGHT_SENSOR_PIN A6
#define DISTANCE_SENSOR_PIN A7
#define IR_SENSOR_PIN 26
#define BUTTON_PIN 32

uint8_t currentMicVolume = 0;
uint8_t currentAmbientBrightness = 0;
uint8_t currentFrontDistance = 0;

char currentIRCommand = ' ';
char previousIRCommand = ' ';

bool buttonOn = false;

unsigned long soundSensorPrevMillis = 0;
unsigned long lightSensorPrevMillis = 0;
unsigned long distanceSensorPrevMillis = 0;
unsigned long irSensorPrevMillis = 0;
unsigned long buttonPrevMillis = 0;

const long soundSensorInterval = 20;
const long lightSensorInterval = 5000;
const long distanceSensorInterval = 500;
const long irSensorInterval = 20;
const long buttonInterval = 200;

extern void readSensors(long currentMillis) {
  if (currentMillis - soundSensorPrevMillis >= soundSensorInterval) {
    soundSensorPrevMillis = currentMillis;
    currentMicVolume = analogRead(SOUND_SENSOR_PIN);
    //Serial.print("Sound Sensor: ");
    //Serial.println(currentMicVolume);
    if(currentMicVolume < 2) {
      //Serial.println("Sound Detected");
      //playStartupMusic();
    }
  }
  if (currentMillis - lightSensorPrevMillis >= lightSensorInterval) {
    lightSensorPrevMillis = currentMillis;
    currentAmbientBrightness = analogRead(LIGHT_SENSOR_PIN);
    //Serial.print("Ambient Brightness: ");
    //Serial.println(currentAmbientBrightness);
  }
  if (currentMillis - distanceSensorPrevMillis >= distanceSensorInterval) {
    distanceSensorPrevMillis = currentMillis;
    currentFrontDistance = analogRead(DISTANCE_SENSOR_PIN);
    //Serial.print("Front Distance: ");
    //Serial.println(currentFrontDistance);
  }
  /*if (currentMillis - irSensorPrevMillis >= irSensorInterval) {
    irSensorPrevMillis = currentMillis;
    if (irrecv.decode(&results)) {
      if (results.value == 0) {
        currentIRCommand = ' ';
      } else {
        currentIRCommand = results.value;
      }
      irrecv.resume();
    }
    //Serial.print("IR Command: ");
   // Serial.println(currentIRCommand);
    if(currentIRCommand == POWER) {
     // Serial.println("Power Button Pressed");
    } 

  }*/
  if (currentMillis - buttonPrevMillis >= buttonInterval) {
    buttonPrevMillis = currentMillis;
    bool currentButtonState = digitalRead(BUTTON_PIN);
    buttonOn = currentButtonState;
    //Serial.print("Button: ");
    //Serial.println(currentButtonState);
  }
  //uint8_t input = MSGEQ7.getVolume();
  //Serial.print("Volume: ");
  //Serial.println(input);
}
//********** END Sensor Reading ***************/
