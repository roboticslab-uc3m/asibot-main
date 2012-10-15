// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "FindRedCan.hpp"

/************************************************************************/
bool FindRedCan::configure(ResourceFinder &rf) {

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("FindRedCan options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    printf("FindRedCan using no additional special options.\n");

    finderThread.setInImg(&inImg);
    finderThread.setOutImg(&outImg);
    finderThread.setOutPort(&outPort);

    finderThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    inImg.open("/findRedCan/img:i");
    outImg.open("/findRedCan/img:o");
    outPort.open("/findRedCan/state:o");

    return true;
}

/*****************************************************************/
double FindRedCan::getPeriod() {
    return 2.0;  // Fixed, in seconds, the slow thread that calls updateModule below
}

/************************************************************************/

bool FindRedCan::updateModule() {
    printf("FindRedCan alive...\n");
    return true;
}

/************************************************************************/

bool FindRedCan::interruptModule() {
    printf("FindRedCan closing...\n");
    outPort.interrupt();
    inImg.interrupt();
    outPort.close();
    inImg.close();
    return true;
}

/************************************************************************/

