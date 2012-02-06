// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "webInterface.h"

/************************************************************************/
WebInterface::WebInterface() { }

/************************************************************************/
bool WebInterface::configure(ResourceFinder &rf) {
    return true;
}

/************************************************************************/
bool WebInterface::updateModule() {
    // printf("Alive\n");
    return true;
}

/************************************************************************/
bool WebInterface::interruptModule() {
    // xPort.disableCallback();
    /*xRpcServer.interrupt();
    cartesianDevice.close();
    xRpcServer.close();*/
    return true;
}

