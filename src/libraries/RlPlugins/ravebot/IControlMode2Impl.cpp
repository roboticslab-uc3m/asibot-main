// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlMode2 Related ------------------------------------

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

