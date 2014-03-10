// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "VisionDepth.hpp"

/************************************************************************/
bool VisionDepth::configure(ResourceFinder &rf) {

    cropSelector = DEFAULT_CROP_SELECTOR;
    ConstString strKinectDevice = DEFAULT_KINECT_DEVICE;
    ConstString strKinectLocal = DEFAULT_KINECT_LOCAL;
    ConstString strKinectRemote = DEFAULT_KINECT_REMOTE;
    watchdog = DEFAULT_WATCHDOG;  // double

    fprintf(stdout,"--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("VisionDepth options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--cropSelector (default: \"%d\")\n",cropSelector);
        printf("\t--kinectDevice (device we create, default: \"%s\")\n",strKinectDevice.c_str());
        printf("\t--kinectLocal (if accesing remote, local port name, default: \"%s\")\n",strKinectLocal.c_str());
        printf("\t--kinectRemote (if accesing remote, remote port name, default: \"%s\")\n",strKinectRemote.c_str());
        printf("\t--watchdog ([s] default: \"%f\")\n",watchdog);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }
    if(rf.check("cropSelector")) cropSelector = rf.find("cropSelector").asInt();
    printf("VisionDepth using cropSelector: %d.\n",cropSelector);
    if(rf.check("kinectDevice")) strKinectDevice = rf.find("kinectDevice").asString();
    if(rf.check("kinectLocal")) strKinectLocal = rf.find("kinectLocal").asString();
    if(rf.check("kinectRemote")) strKinectRemote = rf.find("kinectRemote").asString();
    if(rf.check("watchdog")) watchdog = rf.find("watchdog").asDouble();
    printf("VisionDepth using kinectDevice: %s, kinectLocal: %s, kinectRemote: %s.\n",
        strKinectDevice.c_str(), strKinectLocal.c_str(), strKinectRemote.c_str());
    printf("VisionDepth using watchdog: %f.\n",watchdog);

    if (!rf.check("help")) {
        Property options;
        options.put("device",strKinectDevice);
        options.put("localPortPrefix",strKinectLocal);  //
        options.put("remotePortPrefix",strKinectRemote);  //
        if(rf.check("noMirror")) options.put("noMirror",1);
        while(!dd.open(options)) {
            printf("Waiting for kinectDevice \"%s\"...\n",strKinectDevice.c_str());
            Time::delay(1);
        }
        printf("[VisionDepth] success: kinectDevice available.\n");
        if (! dd.view(kinect) ) fprintf(stderr,"[VisionDepth] warning: kinectDevice bad view.\n");
        else printf("[VisionDepth] success: kinectDevice ok view.\n");

        segmentorThread.setIKinectDeviceDriver(kinect);
        segmentorThread.setOutImg(&outImg);
        segmentorThread.setOutPort(&outPort);

        segmentorThread.setCropSelector(cropSelector);
        if(cropSelector != 0) {
            segmentorThread.setOutCropSelectorImg(&outCropSelectorImg);
            segmentorThread.setInCropSelectorPort(&inCropSelectorPort);
        }
    }

    segmentorThread.init(rf);

    //-----------------OPEN LOCAL PORTS------------//
    outImg.open("/visionDepth/img:o");
    outPort.open("/visionDepth/state:o");
    if(cropSelector != 0) {
        outCropSelectorImg.open("/visionDepth/cropSelector/img:o");
        inCropSelectorPort.open("/visionDepth/cropSelector/state:i");
    }
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
    segmentorThread.stop();
    outImg.interrupt();
    outPort.interrupt();
    if(cropSelector != 0) {
        outCropSelectorImg.interrupt();
        inCropSelectorPort.interrupt();
    }
    dd.close();
    outImg.close();
    outPort.close();
    if(cropSelector != 0) {
        outCropSelectorImg.close();
        inCropSelectorPort.close();
    }
    return true;
}

/************************************************************************/

