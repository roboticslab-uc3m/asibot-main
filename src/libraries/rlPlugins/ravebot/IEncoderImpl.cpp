
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IEncoder Related -----------------------------------------

bool RaveBot::resetEncoder(int j) {
    if ((unsigned int)j>numMotors) return false;
    return setEncoder(j,0.0);
  }

// -----------------------------------------------------------------------------

bool RaveBot::resetEncoders() {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= resetEncoder(i);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoder(int j, double val) {  // encExposed = val;
    setEncRaw(j, val * encRawExposed[j]);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoders(const double *vals) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= setEncoder(i,vals[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoder(int j, double *v) {
    *v=getEncRaw(j);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoders(double *encs) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getEncoder(i,&encs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeed(int j, double *sp) {
    return false;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeeds(double *spds) {
    return false;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderAcceleration(int j, double *spds) {
    return false;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderAccelerations(double *accs) {
    return false;
}

// -----------------------------------------------------------------------------

