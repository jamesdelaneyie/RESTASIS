/*
 * Multi-motor group driver
 *
 * Copyright (C)2017-2019 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "MultiDriver.h"

#define FOREACH_MOTOR(action) for (short i=count-1; i >= 0; i--){action;}

/*
 * Initialize motor parameters
 */
void MultiDriver::startMove(long steps1, long steps2, long steps3, long steps4, long steps5, long steps6, long steps7, long steps8, long steps9, long steps10, long steps11, long steps12){
    long steps[12] = {steps1, steps2, steps3, steps4, steps5, steps6, steps7, steps8, steps9, steps10, steps11, steps12};
    /*
     * Initialize state for all active motors
     */
    FOREACH_MOTOR(
        if (steps[i]){
            motors[i]->startMove(steps[i]);
            event_timers[i] = 1;
        } else {
            event_timers[i] = 0;
        }
    );
    ready = false;
    last_action_end = 0;
    next_action_interval = 1;
}
/*
 * Trigger next step action
 */
long MultiDriver::nextAction(void){
    Motor::delayMicros(next_action_interval, last_action_end);

    // TODO: unroll these loops
    // Trigger all the motors that need it
    FOREACH_MOTOR(
        if (event_timers[i] <= next_action_interval){
            event_timers[i] = motors[i]->nextAction();
        } else {
            event_timers[i] -= next_action_interval;
        }
    );
    last_action_end = micros();

    next_action_interval = 0;
    // Find the time when the next pulse needs to fire
    // this is the smallest non-zero timer value from all active motors
    FOREACH_MOTOR(
        if (event_timers[i] > 0 && (event_timers[i] < next_action_interval || next_action_interval == 0)){
            next_action_interval = event_timers[i];
        }
    );
    ready = (next_action_interval == 0);

    return next_action_interval;
}
/*
 * Optionally, call this to begin braking to stop early
 */
void MultiDriver::startBrake(void){
    FOREACH_MOTOR(
        if (event_timers[i] > 0){
            motors[i]->startBrake();
        }
    )
}
/*
 * Immediate stop
 * Returns the number of steps remaining.
 */
MultiDriver::Steps MultiDriver::stop(void){
    Steps retval = Steps();
    FOREACH_MOTOR(
        if (event_timers[i] > 0){
            retval.steps[i] = motors[i]->stop();
        }
    )
    return retval;
}
/*
 * State querying
 */
bool MultiDriver::isRunning(void){
    bool running = false;
    FOREACH_MOTOR(
        if (motors[i]->getCurrentState() != Motor::STOPPED){
            running = true;
            break;
        }
    )
    return running;
}

/*
 * Initialize pins, calculate timings etc
 */
void MultiDriver::begin(float rpm, short microsteps){
    FOREACH_MOTOR(
        motors[i]->begin(rpm, microsteps);
    )
}

/*
 * Move each motor the requested number of steps, in parallel
 * positive to move forward, negative to reverse, 0 to remain still
 */
void MultiDriver::move(long steps1, long steps2, long steps3, long steps4, long steps5, long steps6, long steps7, long steps8, long steps9, long steps10, long steps11, long steps12){
    startMove(steps1, steps2, steps3, steps4, steps5, steps6, steps7, steps8, steps9, steps10, steps11, steps12);
    while (!ready){
        nextAction();
    }
}

#define CALC_STEPS(i, deg) ((motors[i] && deg) ? motors[i]->calcStepsForRotation(deg) : 0)
void MultiDriver::rotate(long deg1, long deg2, long deg3, long deg4, long deg5, long deg6, long deg7, long deg8, long deg9, long deg10, long deg11, long deg12){
    move(CALC_STEPS(0, deg1), CALC_STEPS(1, deg2), CALC_STEPS(2, deg3), CALC_STEPS(3, deg4), CALC_STEPS(4, deg5), CALC_STEPS(5, deg6), CALC_STEPS(6, deg7), CALC_STEPS(7, deg8), CALC_STEPS(8, deg9), CALC_STEPS(9, deg10), CALC_STEPS(10, deg11), CALC_STEPS(11, deg12));
}

void MultiDriver::rotate(double deg1, double deg2, double deg3, double deg4, double deg5, double deg6, double deg7, double deg8, double deg9, double deg10, double deg11, double deg12){
    move(CALC_STEPS(0, deg1), CALC_STEPS(1, deg2), CALC_STEPS(2, deg3), CALC_STEPS(3, deg4), CALC_STEPS(4, deg5), CALC_STEPS(5, deg6), CALC_STEPS(6, deg7), CALC_STEPS(7, deg8), CALC_STEPS(8, deg9), CALC_STEPS(9, deg10), CALC_STEPS(10, deg11), CALC_STEPS(11, deg12));
}

void MultiDriver::startRotate(long deg1, long deg2, long deg3, long deg4, long deg5, long deg6, long deg7, long deg8, long deg9, long deg10, long deg11, long deg12){
    startMove(CALC_STEPS(0, deg1), CALC_STEPS(1, deg2), CALC_STEPS(2, deg3), CALC_STEPS(3, deg4), CALC_STEPS(4, deg5), CALC_STEPS(5, deg6), CALC_STEPS(6, deg7), CALC_STEPS(7, deg8), CALC_STEPS(8, deg9), CALC_STEPS(9, deg10), CALC_STEPS(10, deg11), CALC_STEPS(11, deg12));
}

void MultiDriver::startRotate(double deg1, double deg2, double deg3, double deg4, double deg5, double deg6, double deg7, double deg8, double deg9, double deg10, double deg11, double deg12){
    startMove(CALC_STEPS(0, deg1), CALC_STEPS(1, deg2), CALC_STEPS(2, deg3), CALC_STEPS(3, deg4), CALC_STEPS(4, deg5), CALC_STEPS(5, deg6), CALC_STEPS(6, deg7), CALC_STEPS(7, deg8), CALC_STEPS(8, deg9), CALC_STEPS(9, deg10), CALC_STEPS(10, deg11), CALC_STEPS(11, deg12));
}

void MultiDriver::setMicrostep(unsigned microsteps){
    FOREACH_MOTOR(motors[i]->setMicrostep(microsteps));
}

void MultiDriver::setRPM(float rpm){
    FOREACH_MOTOR(motors[i]->setRPM(rpm));
}

void MultiDriver::enable(void){
    FOREACH_MOTOR(motors[i]->enable());
}
void MultiDriver::disable(void){
    FOREACH_MOTOR(motors[i]->disable());
}
