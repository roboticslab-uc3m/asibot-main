// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TestRaveBot.h"

/************************************************************************/
TestRaveBot::TestRaveBot() { }

/************************************************************************/
bool TestRaveBot::configure(ResourceFinder &rf) {

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

    //-----------------CHECK ROBOT NAME------------//
    ConstString robot_name;
    if (rf.check("name"))
        robot_name = rf.find("name").asString();
    else robot_name = DEFAULT_ROBOT_NAME;

//    Property options;
    Property options(rf.toString().c_str());  // Little hack to get rf stuff to the robot device
    options.put("device","controlboard");
    options.put("subdevice","ravebot");
    options.put("name","/ravebot");

    robotDevice.open(options);
    
    if (!robotDevice.isValid()) {
        printf("[error] Class instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_RlPlugins_ravebot\" variable is set \"ON\"\n");
        printf("\"SKIP_ravebot is set\" --> should be --> \"ENABLE_ravebot is set\"\n\n");
        // robotDevice.close();  // un-needed?
        return false;
    }

    return true;
}

/************************************************************************/
bool TestRaveBot::updateModule() {
//    printf("Alive\n");
    return true;
}

