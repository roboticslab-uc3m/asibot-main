// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "VisionDepth.hpp"

/************************************************************************/
bool VisionDepth::configure(ResourceFinder &rf) {

    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("VisionDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    fprintf(stdout,"VisionDepth using watchdog: %f.\n",watchdog);

    segmentorThread.setInDepth(&inDepth);
    segmentorThread.setInImg(&inImg);
    segmentorThread.setOutImg(&outImg);
    segmentorThread.setOutPort(&outPort);

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inDepth.open("/visionDepth/depth:i");
    inImg.open("/visionDepth/img:i");
    outImg.open("/visionDepth/img:o");
    outPort.open("/visionDepth/state:o");

    return true;
}

/*****************************************************************/
double VisionDepth::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool VisionDepth::updateModule() {
    printf("VisionDepth alive...\n");
    return true;
}

/************************************************************************/

bool VisionDepth::interruptModule() {
    printf("VisionDepth closing...\n");
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

