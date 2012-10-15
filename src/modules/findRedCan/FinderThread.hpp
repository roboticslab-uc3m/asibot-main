// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __FINDER_THREAD_HPP__
#define __FINDER_THREAD_HPP__

// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// #include "pxToReal.h"  // My own image->real space converter

// #include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/sig/all.h>

#include <yarp/math/Math.h>

#include "cv.h"
#include "highgui.h" // to show windows
#include "BlobResult.h"  // Main blob library include

#define DEFAULT_MS_FINDER 20  // In ms, unmeaningful as ALWAYS gets overwritten by RF
#define DEFAULT_TOLERANCE  5  // amplitude is double!
#define DEFAULT_RGB_R 180  //227
#define DEFAULT_RGB_G 100  //219
#define DEFAULT_RGB_B 120  //250
#define DEFAULT_THRESHOLD 50

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;
// using namespace yarp::math;

class FinderThread : public RateThread {
private:
    BufferedPort<ImageOf<PixelRgb> > *pInImg;
    BufferedPort<ImageOf<PixelRgb> > *pOutImg;  // for testing
    Port *pOutPort;
    //
    int threshold;

public:
    FinderThread() : RateThread(DEFAULT_MS_FINDER) {}  // In ms

    void setInImg(BufferedPort<ImageOf<PixelRgb> > * _pInImg);
    void setOutImg(BufferedPort<ImageOf<PixelRgb> > * _pOutImg);
    void setOutPort(Port *_pOutPort);
    void init(ResourceFinder &rf);
    void run();  // The periodical function
};

#endif

