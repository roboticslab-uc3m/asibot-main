// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PointAtObjectServer.hpp"

using namespace pointatobject;

int main(int argc, char *argv[]) {

    printf("Run \"pointAtObjectServer --help\" for options.\n");
    printf("pointAtObjectServer checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\npointAtObjectServer found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    PointAtObjectServer pointAtObjectServer;
    pointAtObjectServer.init();  // init() is a blocking function
    pointAtObjectServer.close();

    return 0;
}

