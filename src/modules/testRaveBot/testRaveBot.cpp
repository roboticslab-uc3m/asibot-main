// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "testRaveBot.h"

/************************************************************************/
testRaveBot::testRaveBot() { }

/************************************************************************/
bool testRaveBot::configure(ResourceFinder &rf) {

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

    //-----------------CHECK ROBOT NAME------------//
    ConstString robot_name;
    if (rf.check("name"))
        robot_name = rf.find("name").asString();
    else robot_name = DEFAULT_ROBOT_NAME;

    Property options;
    options.put("device","controlboard");
    options.put("subdevice","ravebot");
    options.put("name","/ravebot");

    robotDevice.open(options);
    
    if (!robotDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlPlugins_ravebot\" variable is set \"ON\"\n");
        printf("\"SKIP_ravebot is set\" --> should be --> \"ENABLE_ravebot is set\"\n\n");
        // robotDevice.close();  // un-needed?
        return false;
    }

    return true;
}

/************************************************************************/
bool testRaveBot::updateModule() {
//    printf("Alive\n");
    return true;
}

