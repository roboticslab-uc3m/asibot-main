// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CartesianBot.h"

// -----------------------------------------------------------------------------

bool CartesianBot::fwdKin(const double inDeg[NUM_MOTORS], yarp::sig::Vector &x, yarp::sig::Vector &o) {
    for (int i=0; i<NUM_MOTORS; i++)
        inRad[i]=toRad(inDeg[i]);
    double prP = A1*sin(inRad[1])+A2*sin(inRad[1]+inRad[2])+A3*sin(inRad[1]+inRad[2]+inRad[3]); // P = prime
    double phP = A1*cos(inRad[1])+A2*cos(inRad[1]+inRad[2])+A3*cos(inRad[1]+inRad[2]+inRad[3]);
    double oyP = inDeg[1] + inDeg[2] + inDeg[3];  // [deg]
    x.resize(3);
    x[0] = prP*cos(inRad[0]);
    x[1] = prP*sin(inRad[0]);
    x[2] = phP+A0;  // pz = pzP
    o.resize(2);
    o[0] = oyP; // = pitchP
    o[1] = inDeg[4];  // = ozPP
    return true;
}

// -----------------------------------------------------------------------------

