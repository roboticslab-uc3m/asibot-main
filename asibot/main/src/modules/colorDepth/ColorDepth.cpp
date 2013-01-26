// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorDepth.hpp"

/************************************************************************/
bool ColorDepth::configure(ResourceFinder &rf) {

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("ColorDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("ColorDepth using no additional special options.\n");

    segmentorThread.setInImg(&inImg);
    segmentorThread.setOutImg(&outImg);
    segmentorThread.setOutPort(&outPort);

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inImg.open("/colorDepth/img:i");
    outImg.open("/colorDepth/img:o");
    outPort.open("/colorDepth/state:o");

    return true;
}

/*****************************************************************/
double ColorDepth::getPeriod() {
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/

bool ColorDepth::updateModule() {
    printf("ColorDepth alive...\n");
    return true;
}

/************************************************************************/

bool ColorDepth::interruptModule() {
    printf("ColorDepth closing...\n");
    outPort.interrupt();
    inImg.interrupt();
    outPort.close();
    inImg.close();
    return true;
}

/************************************************************************/

