// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __INLINE_MATH__
#define __INLINE_MATH__

//#include "math.h"

/**
* Simple function to pass from radians to degrees.
* @param inRad angle value in radians.
* @return angle value in degrees.
*/
double toDeg(const double inRad) {
    return (inRad * 180.0 / M_PI);  // return (inRad * 180.0 / 3.14159265);
}

/**
* Simple function to pass from degrees to radians. 
* @param inDeg angle value in degrees.
* @return angle value in radians.
*/
double toRad(const double inDeg) {
    return (inDeg * M_PI / 180.0);  // return (inDeg * 3.14159265 / 180.0);
}

#endif  // __INLINE_MATH__

