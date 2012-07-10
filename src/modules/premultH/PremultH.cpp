// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PremultH.hpp"

/************************************************************************/
bool PremultH::updateModule() {
    printf("PremultH alive...\n");
    return true;
}

/************************************************************************/
double PremultH::getPeriod() {
    return watchdog;  // [s]
}

/************************************************************************/

bool PremultH::configure(ResourceFinder &rf) {
    watchdog = DEFAULT_WATCHDOG;  // double

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
       printf("PremultH Options:\n");
       printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
    }
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    printf("PremultH using watchdog [s]: %f.\n",watchdog);

    // yarp::sig::Matrix ymH(4,4);
    H.resize(4,4);
    ConstString ycsH("H");
    if(!getMatrixFromProperties(rf,ycsH,H)){
        H.eye();
        printf("PremultH using default H: H = I\n");
    }
    else printf("PremultH using custom H:\n%s\n",H.toString().c_str());

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
       return false;
    }

    outPort.open("/out");
    premultPorts.setOutPort(&outPort);
    premultPorts.setH(&H);
    premultPorts.open("/in");
    premultPorts.useCallback();
    return true;
}

/************************************************************************/

bool PremultH::interruptModule() {
    premultPorts.disableCallback();
    premultPorts.close();
    outPort.close();
    return true;
}

/************************************************************************/

bool PremultH::getMatrixFromProperties(Searchable &options, ConstString &tag, yarp::sig::Matrix &H) {

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
}

/************************************************************************/

