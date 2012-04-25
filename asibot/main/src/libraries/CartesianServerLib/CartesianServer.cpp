// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CartesianServer.h"

/************************************************************************/
CartesianServer::CartesianServer() { }

/************************************************************************/
bool CartesianServer::configure(ResourceFinder &rf) {

    ConstString prefix = DEFAULT_PREFIX;
    ConstString controller = DEFAULT_CONTROLLER;
    ConstString robotRemote = DEFAULT_ROBOTREMOTE;
    ConstString robotLocal = DEFAULT_ROBOTLOCAL;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("CartesianServer options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--prefix (port name prefix, default: \"%s\")\n",prefix.c_str());
        printf("\t--controller (cartesian controller device, default: \"%s\")\n",controller.c_str());
        printf("\t--robotRemote (port to whom we connect for movj, default: \"%s\")\n",robotRemote.c_str());
        printf("\t--robotLocal (port we open to connect for movj, default: \"%s\")\n",robotLocal.c_str());
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (rf.check("prefix")) prefix = rf.find("prefix").asString();
    if (rf.check("controller")) controller = rf.find("controller").asString();
    if (rf.check("robotRemote")) robotRemote = rf.find("robotRemote").asString();
    if (rf.check("robotLocal")) robotLocal = rf.find("robotLocal").asString();
    printf("CartesianServer using prefix: %s, controller: %s.\n",prefix.c_str(),controller.c_str());
    printf("CartesianServer using robotRemote: %s, robotLocal: %s.\n",robotRemote.c_str(),robotLocal.c_str());

    //------------------------------CARTESIAN--------------------------------//
    Property options(rf.toString());  // Little hack to get rf stuff to the cartesian device
    options.put("device",controller);
    cartesianDevice.open(options);
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
        return false;
    }
    bool ok = cartesianDevice.view(icart);
    if (!ok) {
        printf("[warning] CartesianServer problems acquiring cartesian interface\n");
        return false;
    } else printf("[success] CartesianServer acquired cartesian interface\n");

    //--------------------------------JOINT----------------------------------//
    Property robotOptions(rf.toString());  // Little hack to get rf stuff to the module
    robotOptions.put("device","remote_controlboard");
    robotOptions.put("remote",robotRemote);
    robotOptions.put("local",robotLocal);
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

