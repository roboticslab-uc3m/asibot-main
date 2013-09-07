// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "VisionDepth.hpp"

/************************************************************************/
bool VisionDepth::configure(ResourceFinder &rf) {

    ConstString strKinectDevice = DEFAULT_KINECT_DEVICE;
    ConstString strKinectLocal = DEFAULT_KINECT_LOCAL;
    ConstString strKinectRemote = DEFAULT_KINECT_REMOTE;
    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("VisionDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--kinectDevice (device we create, default: \"%s\")\n",strKinectDevice.c_str());
        printf("\t--kinectLocal (if accesing remote, local port name, default: \"%s\")\n",strKinectLocal.c_str());
        printf("\t--kinectRemote (if accesing remote, remote port name, default: \"%s\")\n",strKinectRemote.c_str());
        printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    if(rf.check("kinectDevice")) strKinectDevice = rf.find("kinectDevice").asString();
    if(rf.check("kinectLocal")) strKinectLocal = rf.find("kinectLocal").asString();
    if(rf.check("kinectRemote")) strKinectRemote = rf.find("kinectRemote").asString();
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    printf("VisionDepth using watchdog: %f.\n",watchdog);

    Property options;
    options.put("device",strKinectDevice);
    options.put("localPortPrefix",strKinectLocal);  //
    options.put("remotePortPrefix",strKinectRemote);  //
    dd.open(options);
    if(!dd.isValid()) {
        printf("kinectDevice not available.\n");
	    dd.close();
        //Network::fini();
        return false;
    }
    printf("kinectDevice available.\n");
    if (! dd.view(kinect) ) fprintf(stderr,"kinectDevice bad view.\n");
    else printf("kinectDevice ok view.\n");

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

