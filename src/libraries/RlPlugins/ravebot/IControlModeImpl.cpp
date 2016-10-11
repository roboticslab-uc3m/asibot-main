// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    printf("[RaveBot] setPositionMode(%d)\n", j);
    if (modePosVel==0) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        fprintf(stderr,"[RaveBot] warning: setPositionMode() return false; failed to stop\n");
        return false;
    }
    modePosVel = 0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setVelocityMode(int j) {
    printf("[RaveBot] setVelocityMode(%d)\n", j);
    if (modePosVel==1) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    modePosVel = 1;  // Set flag to vel.
    return false;
}

// -----------------------------------------------------------------------------

bool RaveBot::setTorqueMode(int j) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::setImpedancePositionMode(int j) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::setImpedanceVelocityMode(int j) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::setOpenLoopMode(int j) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::getControlMode(int j, int *mode) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::getControlModes(int *modes) {
    return false; // unimplemented
}

// -----------------------------------------------------------------------------

