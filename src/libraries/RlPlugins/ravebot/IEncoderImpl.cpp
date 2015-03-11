// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

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
    *v = getEncExposed(j);
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
    // Make it easy, give the current reference speed.
    *sp = velRaw[j] / velRawExposed[j];  // begins to look like we should use semaphores.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderSpeeds(double *spds) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getEncoderSpeed(i,&spds[i]);
    return ok;
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

bool RaveBot::getEncodersTimed(double *encs, double *time) {
    //CD_INFO("\n");  //-- Way too verbose

    bool ok = true;
    for(unsigned int i=0; i < numMotors; i++)
        ok &= getEncoderTimed(i,&(encs[i]),&(time[i]));
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getEncoderTimed(int j, double *encs, double *time) {
    //CD_INFO("(%d)\n",j);  //-- Way too verbose

    getEncoder(j, encs);
    *time = yarp::os::Time::now();

    return true;
}

// -----------------------------------------------------------------------------
