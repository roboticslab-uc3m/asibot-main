
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IEncoder Related -----------------------------------------

bool RaveBot::resetEncoder(int j) {
    encRaw[j]=0.0;
    return true;
  }

// -----------------------------------------------------------------------------

bool RaveBot::resetEncoders() {
    for (unsigned int i=0; i<numMotors; i++)
      encRaw[i]=0.0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoder(int j, double val) {
    encRaw[j]=val;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoders(const double *vals) {
    for (unsigned int i=0; i<numMotors; i++)
      encRaw[i]=vals[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoder(int j, double *v) {
    *v=encRaw[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoders(double *encs) {
    for (unsigned int i=0; i<numMotors; i++)
      encs[i]=encRaw[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeed(int j, double *sp) {
    *sp=velRaw[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeeds(double *spds) {
    for (unsigned int i=0; i<numMotors; i++)
      spds[i]=velRaw[i];
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


