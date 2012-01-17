// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "cartesianServer.h"

/************************************************************************/
cartesianServer::cartesianServer() { }

/************************************************************************/
bool cartesianServer::configure(ResourceFinder &rf) {

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

//    rf.put("device","cartesianbot");

    cartesianDevice.open(rf);
    
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
//        cartesianDevice.close();
        return 0;
    }

}

/************************************************************************/
bool cartesianServer::updateModule() {
//    printf("Alive\n");
    return true;
}

