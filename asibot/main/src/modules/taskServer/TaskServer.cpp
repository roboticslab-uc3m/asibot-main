// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskServer.hpp"

/************************************************************************/
TaskServer::TaskServer() { }

/************************************************************************/
bool TaskServer::configure(ResourceFinder &rf) {

    //ConstString controller = DEFAULT_CONTROLLER;
    //
    /*printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("TaskServer options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--controller (cartesian controller device, default: \"%s\")\n",controller.c_str());
        // Do not exit: let last layer exit so we get help from the complete chain.
    }*/

    //if (rf.check("controller")) controller = rf.find("controller").asString();
    //printf("TaskServer using controller: %s,  prefix: %s.\n",controller.c_str(),prefix.c_str());

    //------------------------------CARTESIAN--------------------------------//
    /*Property options(rf.toString());  // Little hack to get rf stuff to the cartesian device
    options.put("device",controller);
    cartesianDevice.open(options);
    if (!cartesianDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_RlPlugins_cartesianbot\" variable is set \"ON\"\n");
        printf("\"SKIP_cartesianbot is set\" --> should be --> \"ENABLE_cartesianbot is set\"\n\n");
        return false;
    }
    bool ok = cartesianDevice.view(icart);
    if (!ok) {
        fprintf(stderr, "[TaskServer] warning: Problems acquiring cartesian interface.\n");
        return false;
    } else printf("[TaskServer] success: Acquired cartesian interface.\n");*/

    //---------------------CONFIGURE PORTs------------------------
    taskRpcServer.open("/taskServer/rpc:i");
    taskRpcServer.setReader(taskResponder);
    return true;
}

/************************************************************************/
bool TaskServer::updateModule() {
    // printf("Alive\n");
    return true;
}

/************************************************************************/
bool TaskServer::interruptModule() {
    taskRpcServer.interrupt();
    taskRpcServer.close();
    return true;
}

/************************************************************************/

