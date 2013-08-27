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

    Property options;
    options.put("device","KinectDeviceLocal");
    dd.open(options);
    if(!dd.isValid()) {
        printf("KinectDeviceLocal device not available.\n");
	    dd.close();
        //Network::fini();
        return false;
    }
    printf("KinectDeviceLocal device available.\n");
    if (! dd.view(kinect) ) printf("KinectDeviceLocal bad view.\n");
    printf("KinectDeviceLocal ok view.\n");

    segmentorThread.setIKinectDeviceDriver(kinect);
    segmentorThread.setOutImg(&outImg);
    segmentorThread.setOutPort(&outPort);

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
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
    outImg.interrupt();
    outPort.interrupt();
    dd.close();
    outImg.close();
    outPort.close();
    return true;
}

/************************************************************************/

