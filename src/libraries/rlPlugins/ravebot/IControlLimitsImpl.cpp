
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------- IControlLimits Related ------------------------------------

bool RaveBot::setLimits(int axis, double min, double max) {
    if(axis>=int(numMotors)) return false;
    minLimit[axis] = min;
    maxLimit[axis] = max;
    printf("Range of axis %d set to: %f to %f\n",axis,min,max);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getLimits(int axis, double *min, double *max) {
    if(axis>=int(numMotors)) return false;
    *min = minLimit[axis];
    *max = maxLimit[axis];
    printf("Range of axis %d is: %f to %f.\n",axis,*min,*max);
    return true;
}

