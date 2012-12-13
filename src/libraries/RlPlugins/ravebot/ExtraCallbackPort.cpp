// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ExtraCallbackPort.h"

/************************************************************************/

void ExtraCallbackPort::onRead(Bottle& b) {
    //printf("[ExtraCallbackPort] Got %s.\n", b.toString().c_str());
    //printf("[ExtraCallbackPort] dof: %d, bottle.size(): %d.\n", dof, b.size());
    for(int i=0;i<b.size();i++)
        setEncExposed(i,b.get(i).asDouble());
}

/************************************************************************/

