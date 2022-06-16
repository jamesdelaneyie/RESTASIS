//********** Stepper Motors Setup *************/
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"

bool motorsPowered = false;

#define MOTOR_STEPS 200
#define RPM 60
#define MOTOR_ACCEL 80
#define MOTOR_DECEL 30
#define MICROSTEPS 8

//BasicStepperDriver stepper1(MOTOR_STEPS, DIR, STEP);
//LEDS D2, D3, D4
BasicStepperDriver stepper1(MOTOR_STEPS, 5, 6);
BasicStepperDriver stepper2(MOTOR_STEPS, 7, 8);

//LEDS D10, D11, D12
BasicStepperDriver stepper3(MOTOR_STEPS, 13, 14);
BasicStepperDriver stepper4(MOTOR_STEPS, 15, 16);

//LEDS D19, D20, D21
BasicStepperDriver stepper5(MOTOR_STEPS, 21, 22);
BasicStepperDriver stepper6(MOTOR_STEPS, 23, 24);

//LEDS D51, D52, D53
BasicStepperDriver stepper7(MOTOR_STEPS, 51, 52); //Not Working 
BasicStepperDriver stepper8(MOTOR_STEPS, 49, 50); 
/* BasicStepperDriver stepper7(MOTOR_STEPS, 49, 50); BasicStepperDriver stepper8(MOTOR_STEPS, 51, 52);*/

//LEDS D45, D46, D47
BasicStepperDriver stepper9(MOTOR_STEPS, 41, 42);
BasicStepperDriver stepper10(MOTOR_STEPS, 43, 44);

//LEDS A13, A14, A15
BasicStepperDriver stepper11(MOTOR_STEPS, 66, 65);
BasicStepperDriver stepper12(MOTOR_STEPS, 64, 63);


//62-A8  STEP
//63-A9  DIR
//64-A10 STEP
//65-A11 DIR 
//66-A12 —————
//67-A13-K5 LED
//68-A14-K6 LED
//69-A15-K7 LED


SyncDriver controllerA(stepper1, stepper2, stepper3, stepper4, stepper5, stepper6, stepper7, stepper8, stepper9, stepper10, stepper11, stepper12);



extern void setupMotors() {
  stepper1.begin(RPM, MICROSTEPS);
  stepper1.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper2.begin(RPM, MICROSTEPS);
  stepper2.setSpeedProfile(stepper2.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper3.begin(RPM, MICROSTEPS);
  stepper3.setSpeedProfile(stepper3.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper4.begin(RPM, MICROSTEPS);
  stepper4.setSpeedProfile(stepper4.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper5.begin(RPM, MICROSTEPS);
  stepper5.setSpeedProfile(stepper5.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper6.begin(RPM, MICROSTEPS);
  stepper6.setSpeedProfile(stepper6.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper7.begin(RPM, MICROSTEPS);
  stepper7.setSpeedProfile(stepper7.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper8.begin(RPM, MICROSTEPS);
  stepper8.setSpeedProfile(stepper8.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper9.begin(RPM, MICROSTEPS);
  stepper9.setSpeedProfile(stepper9.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper10.begin(RPM, MICROSTEPS);
  stepper10.setSpeedProfile(stepper10.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper11.begin(RPM, MICROSTEPS);
  stepper11.setSpeedProfile(stepper11.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper12.begin(RPM, MICROSTEPS);
  stepper12.setSpeedProfile(stepper12.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  motorsPowered = true;
}