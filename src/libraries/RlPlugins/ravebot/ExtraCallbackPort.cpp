// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ExtraCallbackPort.h"

/************************************************************************/

void ExtraCallbackPort::onRead(Bottle& b) {
    printf("[ExtraCallbackPort] Got %s\n", b.toString().c_str());
}

/************************************************************************/

