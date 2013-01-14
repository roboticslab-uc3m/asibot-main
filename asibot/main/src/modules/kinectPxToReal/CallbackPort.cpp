// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CallbackPort.hpp"

/************************************************************************/
void CallbackPort::setSharedArea(SharedArea* _pMem) {
    pMem = _pMem;
}

/************************************************************************/
void CallbackPort::setOutPort(Port* _outPort) {
    outPort = _outPort;
}

/************************************************************************/

void CallbackPort::onRead(Bottle& b) {
    printf("[CallbackPort] Got %s\n", b.toString().c_str());
}

/************************************************************************/
