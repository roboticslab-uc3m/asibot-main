// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ----------------------------------------------------------------------------

void RaveBot::setEncRaw(const int Index, const double Position) {
    // printf("[SharedArea] setEncRaw.\n");
    encRawMutex.wait();
    encRaw[Index] = Position;
    encRawMutex.post();
}

// ----------------------------------------------------------------------------

double RaveBot::getEncRaw(const int Index) {
    // printf("[SharedArea] getEncRaw.\n");
    double Position;
    encRawMutex.wait();
    Position = encRaw[Index];
    encRawMutex.post();
    return Position;
}

// ----------------------------------------------------------------------------

double RaveBot::getEncExposed(const int Index) {
    double RawPosition;
    encRawMutex.wait();
    RawPosition = encRaw[Index];
    encRawMutex.post();
    // printf("[SharedArea] get.\n");
    return RawPosition / encRawExposed[Index];
}

// ----------------------------------------------------------------------------

