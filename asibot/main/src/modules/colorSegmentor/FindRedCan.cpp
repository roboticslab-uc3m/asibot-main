// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorSegmentor.hpp"

/************************************************************************/
bool ColorSegmentor::configure(ResourceFinder &rf) {

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("ColorSegmentor options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("ColorSegmentor using no additional special options.\n");

    finderThread.setInImg(&inImg);
    finderThread.setOutImg(&outImg);
    finderThread.setOutPort(&outPort);

    finderThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inImg.open("/colorSegmentor/img:i");
    outImg.open("/colorSegmentor/img:o");
    outPort.open("/colorSegmentor/state:o");

    return true;
}

/*****************************************************************/
double ColorSegmentor::getPeriod() {
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/

bool ColorSegmentor::updateModule() {
    printf("ColorSegmentor alive...\n");
    return true;
}

/************************************************************************/

bool ColorSegmentor::interruptModule() {
    printf("ColorSegmentor closing...\n");
    outPort.interrupt();
    inImg.interrupt();
    outPort.close();
    inImg.close();
    return true;
}

/************************************************************************/

