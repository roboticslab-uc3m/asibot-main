// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "cartesianServer.h"

/************************************************************************/
CartesianServer::CartesianServer() { }

/************************************************************************/
bool CartesianServer::configure(ResourceFinder &rf) {

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

    //-----------------CHECK CONTROLLER NAME------------//
    ConstString controller_name;
    if (rf.check("controller"))
        controller_name = rf.find("controller").asString();
    else controller_name = DEFAULT_CONTROLLER;

    Property options(rf.toString());  // Little hack to get rf stuff to the module
    options.put("device","cartesianbot");
    cartesianDevice.open(options);
    
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
        // cartesianDevice.close();  // un-needed?
        return false;
    }

    bool ok = cartesianDevice.view(icart);
    if (!ok) {
        printf("[warning] Problems acquiring cartesian interface\n");
        return false;
    } else printf("[success] cartesianServer acquired cartesian interface\n");

    Property robotOptions(rf.toString());  // Little hack to get rf stuff to the module
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("local","/cartesianServer");
    robotOptions.put("remote","/ravebot");
    robotDevice.open(robotOptions);
    
    if (!robotDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("[error] robotDevice not valid.\n\n");
        // robotDevice.close();  // un-needed?
        return false;
    }

    bool ok2 = robotDevice.view(ipos);
    if (!ok2) {
        printf("[warning] Problems acquiring robot interfaces\n");
        return false;
    } else printf("[success] cartesianServer acquired robot interfaces\n");

    //---------------------CONFIGURE PORT(s)------------------------//
    xResponder.setPositionInterface(ipos);
    xResponder.setCartesianInterface(icart);
    xRpcServer.open("/cartesianServer/rpc:i");
    xRpcServer.setReader(xResponder);
    xPort.open("/cartesianServer/command:i");
    xPort.useCallback();
    return true;
}

/************************************************************************/
bool CartesianServer::updateModule() {
    // printf("Alive\n");
    return true;
}

/************************************************************************/
bool CartesianServer::interruptModule() {
    xRpcServer.interrupt();
    xPort.disableCallback();
    cartesianDevice.close();
    xRpcServer.close();
    xPort.close();
    return true;
}

