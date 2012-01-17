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

    //-----------------CHECK CONTROLLER NAME------------//
    ConstString controller_name;
    if (rf.check("controller"))
        controller_name = rf.find("controller").asString();
    else controller_name = DEFAULT_CONTROLLER;

    Property options;
    options.put("device","cartesianbot");

    cartesianDevice.open(options);
    
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
        // cartesianDevice.close();  // un-needed?
        return false;
    }

    return true;
}

/************************************************************************/
bool cartesianServer::updateModule() {
//    printf("Alive\n");
    return true;
}

