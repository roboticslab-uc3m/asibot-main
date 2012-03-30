
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IEncoder Related -----------------------------------------

bool RaveBot::resetEncoder(int j) {
    realDeg[j]=0.0;
    return true;
  }

// -----------------------------------------------------------------------------

bool RaveBot::resetEncoders() {
    for (unsigned int i=0; i<numMotors; i++)
      realDeg[i]=0.0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoder(int j, double val) {
    realDeg[j]=val;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoders(const double *vals) {
    for (unsigned int i=0; i<numMotors; i++)
      realDeg[i]=vals[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoder(int j, double *v) {
    *v=realDeg[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoders(double *encs) {
    for (unsigned int i=0; i<numMotors; i++)
      encs[i]=realDeg[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeed(int j, double *sp) {
    *sp=jointVel[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeeds(double *spds) {
    for (unsigned int i=0; i<numMotors; i++)
      spds[i]=jointVel[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderAcceleration(int j, double *spds) {
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderAccelerations(double *accs) {
    return true;
}


