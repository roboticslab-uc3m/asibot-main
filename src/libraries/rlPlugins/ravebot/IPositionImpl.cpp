
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------- IPositionControl Related --------------------------------

bool RaveBot::getAxes(int *ax) {
    *ax = NUM_MOTORS;
    printf("RaveBot reporting %d axes are present\n", *ax);
    return true;
}

