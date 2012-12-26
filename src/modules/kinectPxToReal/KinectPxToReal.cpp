// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "KinectPxToReal.hpp"

/************************************************************************/
bool KinectPxToReal::updateModule() {
    printf("KinectPxToReal alive...\n");
    return true;
}

/************************************************************************/
double KinectPxToReal::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool KinectPxToReal::configure(ResourceFinder &rf) {
    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if(rf.check("help")) {
       printf("KinectPxToReal Options:\n");
       printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
    }
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    fprintf(stdout,"KinectPxToReal using watchdog [s]: %f.\n",watchdog);


    fprintf(stdout,"--------------------------------------------------------------\n");
    if(rf.check("help")) {
       return false;
    }

    depthPort.setSharedArea(&mem);
    callbackPort.setSharedArea(&mem);
    callbackPort.setOutPort(&outPort);
    
    //-----------------OPEN LOCAL PORTS------------//
    depthPort.open("/kinectPxToReal/depth:i");
    callbackPort.open("/kinectPxToReal/state:i");
    outPort.open("/kinectPxToReal/state:o");

    depthPort.useCallback();
    callbackPort.useCallback();

    return true;
}

/************************************************************************/

bool KinectPxToReal::interruptModule() {
    callbackPort.disableCallback();
    depthPort.interrupt();
    callbackPort.interrupt();
    outPort.interrupt();
    depthPort.close();
    callbackPort.close();
    outPort.close();
    return true;
}

/************************************************************************/

