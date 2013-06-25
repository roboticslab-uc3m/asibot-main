// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SEGMENTOR_THREAD_HPP__
#define __SEGMENTOR_THREAD_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/RateThread.h>

#include <yarp/sig/all.h>

#include <yarp/math/Math.h>

#include "cv.h"
//#include "highgui.h" // to show windows

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "TravisLib.hpp"
#include "TinyMath.hpp"

#define DEFAULT_FX          640     //
#define DEFAULT_FY          640     //
#define DEFAULT_CX          320     //
#define DEFAULT_CY          240     //

#define DEFAULT_HEIGHT         0     // Base TransZ [m]
#define DEFAULT_PAN            0     // Base RotZ [deg]
#define DEFAULT_TILT           0     // Extra RotY [deg]

#define DEFAULT_ALGORITHM "blueMinusRed"
#define DEFAULT_LOCATE "centroid"
#define DEFAULT_MAX_NUM_BLOBS 1
#define DEFAULT_OUT_FEATURES_FORMAT 0
#define DEFAULT_OUT_IMAGE 1
#define DEFAULT_RATE_MS 20
#define DEFAULT_SEE_BOUNDING 3
#define DEFAULT_THRESHOLD 55

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

class SegmentorThread : public RateThread {
private:
    BufferedPort<ImageOf<PixelFloat> > *pInDepth;
    BufferedPort<ImageOf<PixelRgb> > *pInImg;
    BufferedPort<ImageOf<PixelRgb> > *pOutImg;  // for testing
    Port *pOutPort;
    //
    ConstString algorithm;
    ConstString locate;
    int maxNumBlobs;
    Bottle outFeatures;
    int outFeaturesFormat;
    int outImage;
    int seeBounding;
    int threshold;
    //
    double fx,fy,cx,cy;
    //
    double height, pan, tilt;

public:
    SegmentorThread() : RateThread(DEFAULT_RATE_MS) {}

    void setInDepth(BufferedPort<ImageOf<PixelFloat> > * _pInDepth);
    void setInImg(BufferedPort<ImageOf<PixelRgb> > * _pInImg);
    void setOutImg(BufferedPort<ImageOf<PixelRgb> > * _pOutImg);
    void setOutPort(Port *_pOutPort);
    void init(ResourceFinder &rf);
    void run();  // The periodical function
};

#endif  // __SEGMENTOR_THREAD_HPP__

