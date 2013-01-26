// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorDepth.hpp"

/************************************************************************/
bool ColorDepth::configure(ResourceFinder &rf) {

    double fx = DEFAULT_FX;
    double fy = DEFAULT_FY;
    double cx = DEFAULT_CX;
    double cy = DEFAULT_CY;
    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("ColorDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--fx (default: \"%f\")\n",fx);
        printf("\t--fy (default: \"%f\")\n",fy);
        printf("\t--cx (default: \"%f\")\n",cx);
        printf("\t--cy (default: \"%f\")\n",cy);
        printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    if(rf.check("fx")) fx = rf.find("fx").asDouble();
    if(rf.check("fy")) fy = rf.find("fy").asDouble();
    if(rf.check("cx")) cx = rf.find("cx").asDouble();
    if(rf.check("cy")) cy = rf.find("cy").asDouble();
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    fprintf(stdout,"ColorDepth using fx: %f, fy: %f, cx: %f, cy: %f.\n",fx,fy,cx,cy);
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
    outPort.interrupt();
    inImg.interrupt();
    inDepth.interrupt();
    outPort.close();
    inImg.close();
    inDepth.close();
    return true;
}

/************************************************************************/

