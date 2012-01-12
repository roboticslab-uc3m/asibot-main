
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IEncoder Related -----------------------------------------

bool RaveBot::resetEncoder(int j) {
    real_degrees[j]=0.0;
    return true;
  }

// -----------------------------------------------------------------------------

bool RaveBot::resetEncoders() {
    for (unsigned int i=0; i<NUM_MOTORS; i++)
      real_degrees[i]=0.0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoder(int j, double val) {
    real_degrees[j]=val;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setEncoders(const double *vals) {
    for (unsigned int i=0; i<NUM_MOTORS; i++)
      real_degrees[i]=vals[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoder(int j, double *v) {
    *v=real_degrees[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoders(double *encs) {
    for (unsigned int i=0; i<NUM_MOTORS; i++)
      encs[i]=real_degrees[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeed(int j, double *sp) {
    *sp=joint_vel[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeeds(double *spds) {
    for (unsigned int i=0; i<NUM_MOTORS; i++)
      spds[i]=joint_vel[i];
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


