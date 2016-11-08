// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    printf("[RaveBot] setPositionMode(%d)\n", j);
    if (!indexWithinRange(j)) return false;
    if (vModePosVel[j]==VOCAB_CM_POSITION) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        fprintf(stderr,"[RaveBot] warning: setPositionMode() return false; failed to stop\n");
        return false;
    }
    vModePosVel[j] = VOCAB_CM_POSITION;  // Set flag to pos.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setVelocityMode(int j) {
    printf("[RaveBot] setVelocityMode(%d)\n", j);
    if (!indexWithinRange(j)) return false;
    if (vModePosVel[j]==VOCAB_CM_VELOCITY) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    vModePosVel[j] = VOCAB_CM_VELOCITY;  // Set flag to vel.
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
    if (!indexWithinRange(j)) return false;
    *mode = vModePosVel[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getControlModes(int *modes) {
    for(unsigned int motor=0;motor<numMotors;motor++) {
        modes[motor] = vModePosVel[motor];
    }
    return true;
}

// -----------------------------------------------------------------------------

