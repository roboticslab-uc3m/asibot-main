// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CartesianServer.h"

/************************************************************************/
CartesianServer::CartesianServer() { }

/************************************************************************/
bool CartesianServer::configure(ResourceFinder &rf) {

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

    //------------------------------CARTESIAN--------------------------------//
    Property options(rf.toString());  // Little hack to get rf stuff to the cartesian device
    options.put("device",DEFAULT_CONTROLLER);
    cartesianDevice.open(options);
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
        return false;
    }
    bool ok = cartesianDevice.view(icart);
    if (!ok) {
        printf("[warning] Problems acquiring cartesian interface\n");
        return false;
    } else printf("[success] cartesianServer acquired cartesian interface\n");

    //--------------------------------JOINT----------------------------------//
    if((!rf.check("robotRemote"))||(!rf.check("robotLocal"))){
        printf("[error] robotRemote and must be defined to configure() CartesianServer\n");
        cartesianDevice.close();
        return false;
    }
    Property robotOptions(rf.toString());  // Little hack to get rf stuff to the module
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("remote",rf.find("robotRemote").asString());
    robotOptions.put("local",rf.find("robotLocal").asString());
    robotDevice.open(robotOptions);
    if (!robotDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("[error] robotRemote not valid, has a remotely accessible device been open?\n\n");
        return false;
    }
    bool ok2 = robotDevice.view(ipos);
    if (!ok2) {
        printf("[warning] Problems acquiring robot interfaces\n");
        return false;
    } else printf("[success] cartesianServer acquired robot interfaces\n");

    //---------------------CONFIGURE PORTs------------------------
    ConstString prefix;
    if(!rf.check("prefix")) {
        printf("[warning] no prefix found.\n");
    } else prefix = rf.find("prefix").asString();
    xResponder.setPositionInterface(ipos);
    xResponder.setCartesianInterface(icart);
    ConstString xRpcServerStr(prefix);
    xRpcServerStr += "/cartesianServer/rpc:i";
    xRpcServer.open(xRpcServerStr);
    xRpcServer.setReader(xResponder);
    xPort.setPositionInterface(ipos);
    xPort.setCartesianInterface(icart);
    ConstString xPortStr(prefix);
    xPortStr += "/cartesianServer/command:i";
    xPort.open(xPortStr);
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

