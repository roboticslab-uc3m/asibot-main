// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __INLINE_MATH__
#define __INLINE_MATH__

//#include "math.h"

// ----------------------------------------------------------------------------

double toDeg(const double inRad) {
    return (inRad * 180.0 / M_PI);  // return (inRad * 180.0 / 3.14159265);
}

// ----------------------------------------------------------------------------

double toRad(const double inDeg) {
    return (inDeg * M_PI / 180.0);  // return (inDeg * 3.14159265 / 180.0);
}

// ----------------------------------------------------------------------------

#ifndef __INLINE_MATH__

