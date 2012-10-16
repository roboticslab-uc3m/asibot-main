// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __FINDER_THREAD_HPP__
#define __FINDER_THREAD_HPP__

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

#define DEFAULT_ALGORITHM "redMinusGreen"
#define DEFAULT_MAX_NUM_BLOBS 1
#define DEFAULT_THRESHOLD 50

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

class SegmentorThread : public RateThread {
private:
    BufferedPort<ImageOf<PixelRgb> > *pInImg;
    BufferedPort<ImageOf<PixelRgb> > *pOutImg;  // for testing
    Port *pOutPort;
    //
    ConstString algorithm;
    int maxNumBlobs;
    int threshold;

public:
    SegmentorThread() : RateThread(DEFAULT_MS_FINDER) {}  // In ms

    void setInImg(BufferedPort<ImageOf<PixelRgb> > * _pInImg);
    void setOutImg(BufferedPort<ImageOf<PixelRgb> > * _pOutImg);
    void setOutPort(Port *_pOutPort);
    void init(ResourceFinder &rf);
    void run();  // The periodical function
};

#endif

