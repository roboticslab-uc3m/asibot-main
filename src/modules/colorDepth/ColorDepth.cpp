// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorDepth.hpp"

/************************************************************************/
bool ColorDepth::configure(ResourceFinder &rf) {

    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("ColorDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    fprintf(stdout,"ColorDepth using watchdog: %f.\n",watchdog);

    segmentorThread.setInDepth(&inDepth);
    segmentorThread.setInImg(&inImg);
    segmentorThread.setOutImg(&outImg);
    segmentorThread.setOutPort(&outPort);

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inDepth.open("/colorDepth/depth:i");
    inImg.open("/colorDepth/img:i");
    outImg.open("/colorDepth/img:o");
    outPort.open("/colorDepth/state:o");

    return true;
}

/*****************************************************************/
double ColorDepth::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool ColorDepth::updateModule() {
    printf("ColorDepth alive...\n");
    return true;
}

/************************************************************************/

bool ColorDepth::interruptModule() {
    printf("ColorDepth closing...\n");
    inDepth.interrupt();
    inImg.interrupt();
    outImg.interrupt();
    outPort.interrupt();
    inDepth.close();
    inImg.close();
    outImg.close();
    outPort.close();
    return true;
}

/************************************************************************/

