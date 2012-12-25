// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SharedArea.hpp"

/************************************************************************/
void SharedArea::setF(const Vector& _f) {
    fMutex.wait();
    f = _f;
    fMutex.post();
}

/************************************************************************/
void SharedArea::setQ(const Vector& _q) {
    qMutex.wait();
    q = _q;
    qMutex.post();
}

/************************************************************************/
Vector SharedArea::getQ() {
    qMutex.wait();
    Vector _q=q;
    qMutex.post();
    return _q;
}

/************************************************************************/
Vector SharedArea::getF() {
    fMutex.wait();
    Vector _f=f;
    fMutex.post();
    return _f;
}

/************************************************************************/

