// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlLimits Related ------------------------------------

bool RaveBot::setLimits(int axis, double min, double max) {
    if(axis>=int(numMotors)) return false;
    minLimit[axis] = min;
    maxLimit[axis] = max;
    printf("[RaveBot] Range of axis %d set to: %f to %f\n",axis,min,max);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getLimits(int axis, double *min, double *max) {
    if(axis>=int(numMotors)) return false;
    *min = minLimit[axis];
    *max = maxLimit[axis];
    printf("[RaveBot] Range of axis %d read: %f to %f.\n",axis,*min,*max);
    return true;
}

// -----------------------------------------------------------------------------

