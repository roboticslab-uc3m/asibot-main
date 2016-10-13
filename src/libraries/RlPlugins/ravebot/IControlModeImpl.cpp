// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    printf("[RaveBot] setPositionMode(%d)\n", j);
    if ((unsigned int)j>numMotors) return false;
    if (vModePosVel[j]==0) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        fprintf(stderr,"[RaveBot] warning: setPositionMode() return false; failed to stop\n");
        return false;
    }
    vModePosVel[j] = 0;  // Set flag to pos.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setVelocityMode(int j) {
    printf("[RaveBot] setVelocityMode(%d)\n", j);
    if ((unsigned int)j>numMotors) return false;
    if (vModePosVel[j]==1) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    vModePosVel[j] = 1;  // Set flag to vel.
    return true;
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

