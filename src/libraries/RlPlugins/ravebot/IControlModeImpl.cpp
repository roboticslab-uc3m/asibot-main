// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    if (!indexWithinRange(j)) return false;
    if (vModePosVel[j]==VOCAB_CM_POSITION) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop(j)) {
        fprintf(stderr,"[RaveBot] warning: could not set position mode at joint %d; failed to stop\n", j+1);
        return false;
    }
    vModePosVel[j] = VOCAB_CM_POSITION;  // Set flag to pos.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setVelocityMode(int j) {
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
    int joints[numMotors];
    for (unsigned int i = 0; i < numMotors; i++)
        joints[i] = i;
    return getControlModes(numMotors, joints, modes);
}

// -----------------------------------------------------------------------------

