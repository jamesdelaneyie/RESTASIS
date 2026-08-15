/*
 * Synchronous Multi-motor group driver
 * All motors reach their target at the same time.
 *
 * Copyright (C)2017-2019 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "SyncDriver.h"

#define FOREACH_MOTOR(action) for (short i=count-1; i >= 0; i--){action;}

/*
 * Initialize motor parameters
 */
void SyncDriver::startMove(long steps1, long steps2, long steps3, long steps4, long steps5, long steps6, long steps7, long steps8, long steps9, long steps10, long steps11, long steps12){
    long steps[12] = {steps1, steps2, steps3, steps4, steps5, steps6, steps7, steps8, steps9, steps10, steps11, steps12};
    /*
     * find which motor would take the longest to finish,
     */
    long move_time = 0;
    FOREACH_MOTOR(
        long m = motors[i]->getTimeForMove(labs(steps[i]));
        if (m > move_time){
            move_time = m;
        }
    );
    /*
     * Initialize state for all active motors to complete with <move_time> micros
     */
    FOREACH_MOTOR(
        if (steps[i]){
            motors[i]->startMove(steps[i], move_time);
            event_timers[i] = 1;
        } else {
         event_timers[i] = 0;
       }
    );
    ready = false;
    last_action_end = 0;
    next_action_interval = 1;
}
