// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------ IEncodersTimed Related -----------------------------------------

bool RaveBot::getEncodersTimed(double *encs, double *time) {
    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= getEncoderTimed(i,&(encs[i]),&(time[i]));
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderTimed(int j, double *encs, double *time) {
    getEncoder(j, encs);
    *time = yarp::os::Time::now();

    return true;
}

// -----------------------------------------------------------------------------
