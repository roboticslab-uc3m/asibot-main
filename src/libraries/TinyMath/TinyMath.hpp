// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TINY_MATH__
#define __TINY_MATH__

#include <math.h>  // provides: M_PI ?

#include <stdio.h>  // provides: fprintf, stderr

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>

#include <yarp/math/Math.h>  // provides: eye, operators

using namespace yarp::math;  // else matrix operators wreak havoc

/**
* Simple function to pass from radians to degrees.
* @param inRad angle value in radians.
* @return angle value in degrees.
*/
double toDeg(const double inRad);

/**
* Simple function to pass from degrees to radians. 
* @param inDeg angle value in degrees.
* @return angle value in radians.
*/
double toRad(const double inDeg);

/**
* Thanks [Ugo Pattacini, Serena Ivaldi, Francesco Nori ((iCub ctrllib/math.h))] for this one!
*/
yarp::sig::Matrix axis2dcm(const yarp::sig::Vector &v, unsigned int verbose);

yarp::sig::Matrix asibot2h(const yarp::sig::Vector &x, const yarp::sig::Vector &o, unsigned int verbose);

#endif  // __TINY_MATH__

