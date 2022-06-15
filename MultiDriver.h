/*
 * Multi-motor group driver
 *
 * Copyright (C)2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#ifndef MULTI_DRIVER_H
#define MULTI_DRIVER_H
#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MAX_MOTORS 12    // a reasonable but arbitrary limit
#define Motor BasicStepperDriver
/*
 * Multi-motor group driver class.
 */
class MultiDriver {
protected:
    /*
     * Configuration
     */
    unsigned short count;
    Motor* const *motors;
    /*
     * Generic initializer, will be called by the others
     */
    MultiDriver(const unsigned short count, Motor* const *motors)
    :count(count), motors(motors)
    {};

    /*
     * Movement state
     */
    // ready to start a new move
    bool ready = true;
    // when next state change is due for each motor
    unsigned long event_timers[MAX_MOTORS];
    unsigned long next_action_interval = 0;
    unsigned long last_action_end = 0;

public:
    struct Steps {
        long steps[12];
    };
    /*
     * Two-motor setup
     */
    MultiDriver(Motor& motor1, Motor& motor2)
    :MultiDriver(2, new Motor* const[2]{&motor1, &motor2})
    {};
    /*
     * Three-motor setup (X, Y, Z for example)
     */
    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3)
    :MultiDriver(3, new Motor* const[3]{&motor1, &motor2, &motor3})
    {};
    /*
     * Three-motor setup (X, Y, Z for example)
     */
    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4)
    :MultiDriver(4, new Motor* const[4]{&motor1, &motor2, &motor3, &motor4})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5)
    :MultiDriver(5, new Motor* const[5]{&motor1, &motor2, &motor3, &motor4, &motor5})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6)
    :MultiDriver(6, new Motor* const[6]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7)
    :MultiDriver(7, new Motor* const[7]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7, Motor& motor8)
    :MultiDriver(8, new Motor* const[8]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7, &motor8})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7, Motor& motor8, Motor& motor9)
    :MultiDriver(9, new Motor* const[9]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7, &motor8, &motor9})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7, Motor& motor8, Motor& motor9, Motor& motor10)
    :MultiDriver(10, new Motor* const[10]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7, &motor8, &motor9, &motor10})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7, Motor& motor8, Motor& motor9, Motor& motor10, Motor& motor11)
    :MultiDriver(11, new Motor* const[11]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7, &motor8, &motor9, &motor10, &motor11})
    {};

    MultiDriver(Motor& motor1, Motor& motor2, Motor& motor3, Motor& motor4, Motor& motor5, Motor& motor6, Motor& motor7, Motor& motor8, Motor& motor9, Motor& motor10, Motor& motor11, Motor& motor12)
    :MultiDriver(12, new Motor* const[12]{&motor1, &motor2, &motor3, &motor4, &motor5, &motor6, &motor7, &motor8, &motor9, &motor10, &motor11, &motor12})
    {};

    

    unsigned short getCount(void){
        return count;
    }
    Motor& getMotor(short index){
        return *motors[index];
    }
    /*
     * Initialize pins, calculate timings etc
     */
    void begin(float rpm=60, short microsteps=1);
    /*
     * Move the motors a given number of steps.
     * positive to move forward, negative to reverse
     */
    void move(long steps1, long steps2, long steps3=0, long steps4=0, long steps5=0, long steps6=0, long steps7=0, long steps8=0, long steps9=0, long steps10=0, long steps11=0, long steps12=0);
    void rotate(int deg1, int deg2, int deg3=0, int deg4=0, int deg5=0, int deg6=0, int deg7=0, int deg8=0, int deg9=0, int deg10=0, int deg11=0, int deg12=0){
        rotate((long)deg1, (long)deg2, (long)deg3, (long)deg4, (long)deg5, (long)deg6, (long)deg7, (long)deg8, (long)deg9, (long)deg10, (long)deg11, (long)deg12);
    };
    void rotate(long deg1, long deg2, long deg3=0, long deg4=0, long deg5=0, long deg6=0, long deg7=0, long deg8=0, long deg9=0, long deg10=0, long deg11=0, long deg12=0);
    void rotate(double deg1, double deg2, double deg3=0, double deg4=0, double deg5=0, double deg6=0, double deg7=0, double deg8=0, double deg9=0, double deg10=0, double deg11=0, double deg12=0);

    /*
     * Motor movement with external control of timing
     */
    virtual void startMove(long steps1, long steps2, long steps3=0, long steps4=0, long steps5=0, long steps6=0, long steps7=0, long steps8=0, long steps9=0, long steps10=0, long steps11=0, long steps12=0);
    void startRotate(int deg1, int deg2, int deg3=0, int deg4=0, int deg5=0, int deg6=0, int deg7=0, int deg8=0, int deg9=0, int deg10=0, int deg11=0, int deg12=0){
        startRotate((long)deg1, (long)deg2, (long)deg3, (long)deg4, (long)deg5, (long)deg6, (long)deg7, (long)deg8, (long)deg9, (long)deg10, (long)deg11, (long)deg12);
    };
    void startRotate(long deg1, long deg2, long deg3=0, long deg4=0, long deg5=0, long deg6=0, long deg7=0, long deg8=0, long deg9=0, long deg10=0, long deg11=0, long deg12=0);
    void startRotate(double deg1, double deg2, double deg3=0, double deg4=0, double deg5=0, double deg6=0, double deg7=0, double deg8=0, double deg9=0, double deg10=0, double deg11=0, double deg12=0);
    /*
     * Toggle step and return time until next change is needed (micros)
     */
    virtual long nextAction(void);
    /*
     * Optionally, call this to begin braking to stop early
     */
    void startBrake(void);
    /*
     * Immediate stop
     * Returns the number of steps remaining.
     */
    Steps stop(void);
    /*
     * State querying
     */
    bool isRunning(void);

    /*
     * Set the same microstepping level on all motors
     */
    void setMicrostep(unsigned microsteps);
    /*
     * Set all motors RPM (1-200 is a reasonable range)
     */
    void setRPM(float rpm);
    /*
     * Turn all motors on or off
     */
    void enable(void);
    void disable(void);
};
#endif // MULTI_DRIVER_H
