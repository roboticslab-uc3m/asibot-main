// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>

#include <yarp/os/all.h>

#include "TaskNavigate.h"

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

    return 0;
}


