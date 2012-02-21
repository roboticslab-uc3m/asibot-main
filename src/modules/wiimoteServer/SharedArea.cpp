// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SharedArea.h"

/************************************************************************/
void SharedArea::setCmd(const int& _cmd) {
    cmdMutex.wait();
    cmd = _cmd;
    cmdMutex.post();
}

/************************************************************************/
void SharedArea::setPitch(const double& _pitch) {
    pitchMutex.wait();
    pitch = _pitch;
    pitchMutex.post();
}

/************************************************************************/
void SharedArea::setRoll(const double& _roll) {
    rollMutex.wait();
    roll = _roll;
    rollMutex.post();
}

/************************************************************************/
int SharedArea::getCmd() {
    cmdMutex.wait();
    int _cmd = cmd;
    cmdMutex.post();
    return _cmd;
}

/************************************************************************/
double SharedArea::getPitch() {
    pitchMutex.wait();
    double _pitch = pitch;
    pitchMutex.post();
    return _pitch;
}

/************************************************************************/
double SharedArea::getRoll() {
    rollMutex.wait();
    double _roll = roll;
    rollMutex.post();
    return _roll;
}

