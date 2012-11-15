// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>

#include <yarp/os/all.h>

#include "TaskMakeUp.h"

using namespace yarp::os;

int main(int argc, char *argv[]) {

    printf("WARNING: requires a running instance of RaveBot (i.e. testRaveBot or cartesianServer)\n");
    Network yarp;
    if (!Network::checkNetwork()) {
        fprintf(stderr,"Please start a yarp name server first\n");
        return(-1);
    }

    TaskMakeUp taskMakeUp;
    if(!taskMakeUp.open("/ravebot")) return -1;
    taskMakeUp.run("ASIBOT0","CHAIR0","CHAIR_DOCK0","CONNECTORB","TOOLMAKEUP","BATHROOM0","PALETTE0");  // here hard-coded, best get from *argv
    taskMakeUp.close();

    return 0;
}


