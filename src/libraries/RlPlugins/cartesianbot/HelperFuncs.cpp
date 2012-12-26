
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// -----------------------------------------------------------------------------

double CartesianBot::toDeg(const double inRad) {
//    return (inRad * 180.0 / 3.14159265);
    return (inRad * 180.0 / M_PI);
}

// -----------------------------------------------------------------------------

double CartesianBot::toRad(const double inDeg) {
//    return (inDeg * 3.14159265 / 180.0);
    return (inDeg * M_PI / 180.0);
}

// -----------------------------------------------------------------------------

bool CartesianBot::fwdKin(const double inDeg[NUM_MOTORS], yarp::sig::Vector &x, yarp::sig::Vector &o) {
    for (int i=0; i<NUM_MOTORS; i++)
        realRad[i]=toRad(inDeg[i]);
    double prP = A1*sin(realRad[1])+A2*sin(realRad[1]+realRad[2])+A3*sin(realRad[1]+realRad[2]+realRad[3]); // P = prime
    double phP = A1*cos(realRad[1])+A2*cos(realRad[1]+realRad[2])+A3*cos(realRad[1]+realRad[2]+realRad[3]);
    double oyP = inDeg[1] + inDeg[2] + inDeg[3];  // [deg]
    x.resize(3);
    x[0] = prP*cos(realRad[0]);
    x[1] = prP*sin(realRad[0]);
    x[2] = phP+A0;  // pz = pzP
    o.resize(2);
    o[0] = oyP; // = pitchP
    o[1] = inDeg[4];  // = ozPP
    return true;
}
