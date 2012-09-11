// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ----------------------------------------------------------------------------

void RaveBot::setEncRaw(const int Index, const int Position) {
    // printf("[SharedArea] setEncRaw.\n");
    encRawMutex.wait();
    encRaw[Index] = Position;
    encRawMutex.post();
}

// ----------------------------------------------------------------------------

int RaveBot::getEncRaw(const int Index) {
    // printf("[SharedArea] getEncRaw.\n");
    int Position;
    encRawMutex.wait();
    Position = encRaw[Index];
    encRawMutex.post();
    return Position;
}

// ----------------------------------------------------------------------------

int RaveBot::getEncExposed(const int Index) {
    int RawPosition;
    encRawMutex.wait();
    RawPosition = encRaw[Index];
    encRawMutex.post();
    // printf("[SharedArea] get.\n");
    return RawPosition / encRawExposed[Index];
}

// ----------------------------------------------------------------------------

