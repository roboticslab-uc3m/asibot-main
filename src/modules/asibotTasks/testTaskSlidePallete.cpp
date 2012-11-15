// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>

#include <yarp/os/all.h>

#include "TaskSlidePallete.h"

using namespace yarp::os;

int main(int argc, char *argv[]) {

    printf("WARNING: requires a running instance of RaveBot (i.e. testRaveBot or cartesianServer)\n");
    Network yarp;
    if (!Network::checkNetwork()) {
        fprintf(stderr,"Please start a yarp name server first\n");
        return(-1);
    }

    TaskSlidePallete taskSlidePallete;
    if(!taskSlidePallete.open("/ravebot")) return -1;
    taskSlidePallete.run("ASIBOT0","CHAIR0","CHAIR_DOCK0","CONNECTORB","BATHROOM0","PALETTE0");  // here hard-coded, best get from *argv
    taskSlidePallete.close();

    return 0;
}


