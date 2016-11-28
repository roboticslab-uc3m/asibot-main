// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IControlLimits2 Related ------------------------------------

bool RaveBot::setVelLimits(int axis, double min, double max) {
    return true; // unimplemented
}

// -----------------------------------------------------------------------------

bool RaveBot::getVelLimits(int axis, double *min, double *max) {
    // yarpmotorgui's defauls (partitem.cpp)
    *min = 0;
    *max = 100;
    return true; // unimplemented
}

// -----------------------------------------------------------------------------

