// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>

#include <yarp/os/all.h>

#include "TaskNavigate.h"
#include "TaskCatchTool.h"
#include "TaskSlidePallete.h"
#include "TaskMakeUp.h"

using namespace yarp::os;

int main(int argc, char *argv[]) {

    printf("WARNING: requires a running instance of RaveBot (i.e. testRaveBot or cartesianServer)\n");
    Network yarp;
    if (!Network::checkNetwork()) {
        fprintf(stderr,"Please start a yarp name server first\n");
        return(-1);
    }

    TaskNavigate taskNavigate;
    if(!taskNavigate.open("/ravebot")) return -1;
    taskNavigate.run("ASIBOT0","CHAIR0","DINING_ROOM","BATHROOM0");  // here hard-coded, best get from *argv
    taskNavigate.close();

    TaskCatchTool taskCatchTool;
    if(!taskCatchTool.open("/ravebot")) return -1;
    taskCatchTool.run("ASIBOT0","CHAIR0","CONNECTORB","TOOLMAKEUP");  // here hard-coded, best get from *argv
    taskCatchTool.close();

    TaskSlidePallete taskSlidePallete;
    if(!taskSlidePallete.open("/ravebot")) return -1;
    taskSlidePallete.run("ASIBOT0","CHAIR0","CHAIR_DOCK0","CONNECTORB","BATHROOM0","PALETTE0");  // here hard-coded, best get from *argv
    taskSlidePallete.close();

    TaskMakeUp taskMakeUp;
    if(!taskMakeUp.open("/ravebot")) return -1;
    taskMakeUp.run("ASIBOT0","CHAIR0","CHAIR_DOCK0","CONNECTORB","TOOLMAKEUP","BATHROOM0","PALETTE0");  // here hard-coded, best get from *argv
    taskMakeUp.close();



    return 0;
}


