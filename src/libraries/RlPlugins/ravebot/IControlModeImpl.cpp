// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    printf("[RaveBot] setPositionMode(%d)\n", j);
    if ((unsigned int)j>numMotors) return false;
    if (vModePosVel[j]==VOCAB_POSITION_MODE) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        fprintf(stderr,"[RaveBot] warning: setPositionMode() return false; failed to stop\n");
        return false;
    }
    vModePosVel[j] = VOCAB_POSITION_MODE;  // Set flag to pos.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setVelocityMode(int j) {
    printf("[RaveBot] setVelocityMode(%d)\n", j);
    if ((unsigned int)j>numMotors) return false;
    if (vModePosVel[j]==VOCAB_VELOCITY_MODE) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    vModePosVel[j] = VOCAB_VELOCITY_MODE;  // Set flag to vel.
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

