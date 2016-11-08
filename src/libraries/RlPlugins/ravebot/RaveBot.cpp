// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// -----------------------------------------------------------------------------

bool RaveBot::indexWithinRange(const int& idx) {
    if (idx < 0) {
        fprintf(stderr,"[RaveBot] warning: negative index (%d)\n", idx);
        return false;
    }
    if (idx >= int(numMotors)) {
        fprintf(stderr,"[RaveBot] warning: index out of range (%d >= %d)\n", idx, numMotors);
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
