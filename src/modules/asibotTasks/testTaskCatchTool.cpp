// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>

#include <yarp/os/all.h>

#include "TaskCatchTool.h"

using namespace yarp::os;

int main(int argc, char *argv[]) {

    printf("WARNING: requires a running instance of RaveBot (i.e. testRaveBot or cartesianServer)\n");
    Network yarp;
    if (!Network::checkNetwork()) {
        fprintf(stderr,"Please start a yarp name server first\n");
        return(-1);
    }

    TaskCatchTool taskCatchTool;
    if(!taskCatchTool.open("/ravebot")) return -1;
    taskCatchTool.run("ASIBOT0","CHAIR0","CONNECTORB","TOOLMAKEUP");  // here hard-coded, best get from *argv
    taskCatchTool.close();

    return 0;
}


