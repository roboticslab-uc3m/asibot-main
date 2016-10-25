// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode Related ------------------------------------

bool RaveBot::setPositionMode(int j) {
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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

bool RaveBot::getControlModes(const int n_joints, const int *joints, int *modes) {
    bool ok = true;
    for (int i = 0; i < n_joints; i++)
        ok &= getControlMode(joints[i], &modes[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::setControlMode(const int j, const int mode) {
    if ((unsigned int)j>numMotors) return false;
    switch (mode) {
        case VOCAB_CM_POSITION:
            return setPositionMode(j);
        case VOCAB_CM_VELOCITY:
            return setVelocityMode(j);
        case VOCAB_CM_TORQUE:
            return setTorqueMode(j);
        case VOCAB_CM_IMPEDANCE_POS:
            return setImpedancePositionMode(j);
        case VOCAB_CM_IMPEDANCE_VEL:
            return setImpedanceVelocityMode(j);
        case VOCAB_CM_OPENLOOP:
            return setOpenLoopMode(j);
        default:
            return false; // unrecognized/unsupported
    }
}

// -----------------------------------------------------------------------------

bool RaveBot::setControlModes(const int n_joints, const int *joints, int *modes) {
    bool ok = true;
    for (int i = 0; i < n_joints; i++)
        ok &= setControlMode(joints[i], modes[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::setControlModes(int *modes) {
    int joints[numMotors];
    for (unsigned int i = 0; i < numMotors; i++)
        joints[i] = i;
    return setControlModes(numMotors, joints, modes);
}

// -----------------------------------------------------------------------------

