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

    return true;
}

/************************************************************************/

bool KinectPxToReal::interruptModule() {
    /*inDepth.interrupt();
    inPort.interrupt();
    callbackPort.interrupt();
    inDepth.close();
    inPort.close();
    callbackPort.close();*/
    return true;
}

/************************************************************************/

/*bool KinectPxToReal::getMatrixFromProperties(Searchable &options, ConstString &tag, yarp::sig::Matrix &H) {

    Bottle *bH=options.find(tag).asList();
    if (!bH) return false;
    
    int i=0;
    int j=0;
    H.zero();
    for (int cnt=0; (cnt<bH->size()) && (cnt<H.rows()*H.cols()); cnt++) {    
        H(i,j)=bH->get(cnt).asDouble();
        if (++j>=H.cols()) {
            i++;
            j=0;
        }
    }
    return true;
}*/

/************************************************************************/

