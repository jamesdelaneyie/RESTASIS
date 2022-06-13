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

BasicStepperDriver stepper1(MOTOR_STEPS, 2, 3);
BasicStepperDriver stepper2(MOTOR_STEPS, 4, 5);
BasicStepperDriver stepper3(MOTOR_STEPS, 13, 14);
BasicStepperDriver stepper4(MOTOR_STEPS, 15, 16);
/*BasicStepperDriver stepper5(MOTOR_STEPS, 10, 11);
BasicStepperDriver stepper6(MOTOR_STEPS, 12, 13);
BasicStepperDriver stepper7(MOTOR_STEPS, 14, 15);
BasicStepperDriver stepper8(MOTOR_STEPS, 16, 17);
BasicStepperDriver stepper9(MOTOR_STEPS, 18, 19);
BasicStepperDriver stepper10(MOTOR_STEPS, 20, 21);
BasicStepperDriver stepper11(MOTOR_STEPS, 22, 23);
BasicStepperDriver stepper12(MOTOR_STEPS, 24, 25);*/


SyncDriver controller(stepper1, stepper2);
SyncDriver controller2(stepper3, stepper4);
/*MultiDriver controller2(stepper3, stepper4);
MultiDriver controller3(stepper5, stepper6);
MultiDriver controller4(stepper7, stepper8);
MultiDriver controller5(stepper9, stepper10);
MultiDriver controller6(stepper11, stepper12);*/



extern void setupMotors() {
  stepper1.begin(RPM, MICROSTEPS);
  stepper1.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper2.begin(RPM, MICROSTEPS);
  stepper2.setSpeedProfile(stepper2.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper3.begin(RPM, MICROSTEPS);
  stepper3.setSpeedProfile(stepper3.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper4.begin(RPM, MICROSTEPS);
  stepper4.setSpeedProfile(stepper4.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  controller.rotate(-30, 30);
  controller2.rotate(-30, 30);
  //stepper1.2move(30);
  //stepper1.move(-30);
  //stepper1.rotate(30);
  //stepper1.rotate(-30);

  //stepper2.rotate(-30);
 /* stepper3.begin(RPM, MICROSTEPS);
  stepper3.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  
  stepper4.begin(RPM, MICROSTEPS);
  stepper4.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper5.begin(RPM, MICROSTEPS);
  stepper5.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper6.begin(RPM, MICROSTEPS);
  stepper6.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper7.begin(RPM, MICROSTEPS);
  stepper7.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper8.begin(RPM, MICROSTEPS);
  stepper8.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper9.begin(RPM, MICROSTEPS);
  stepper9.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper10.begin(RPM, MICROSTEPS);
  stepper10.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper11.begin(RPM, MICROSTEPS);
  stepper11.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepper12.begin(RPM, MICROSTEPS);
  stepper12.setSpeedProfile(stepper1.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);*/

  motorsPowered = true;
}