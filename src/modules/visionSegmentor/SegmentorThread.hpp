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

#include "cv.h"
//#include "highgui.h" // to show windows

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "TravisLib.hpp"

#define DEFAULT_ALGORITHM "redMinusGreen"
#define DEFAULT_LOCATE "centroid"
#define DEFAULT_MAX_NUM_BLOBS 1
#define DEFAULT_RATE_MS 20
#define DEFAULT_SEE_BOUNDING 0
#define DEFAULT_THRESHOLD 50

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

using namespace cv;

class SegmentorThread : public RateThread {
private:
    BufferedPort<ImageOf<PixelRgb> > *pInImg;
    BufferedPort<ImageOf<PixelRgb> > *pOutImg;  // for testing
    Port *pOutPort;
    //
    ConstString algorithm;
    ConstString locate;
    int maxNumBlobs;
    int seeBounding;
    int threshold;
    //
    float area, hue_peak, hue_mode, hue_mean, hue_stddev,
        saturation_peak, saturation_mean, saturation_stddev,
        value_peak, value_mode, value_mean, value_stddev, locX, locY,
        rectangularity, axisFirst, axisSecond,
        aspectRatio, solidity, massCenterlocX, massCenterlocY, arc, radius;

public:
    SegmentorThread() : RateThread(DEFAULT_RATE_MS),
        area(-1), hue_peak(-1), hue_mode(-1), hue_mean(-1), hue_stddev(-1),
        saturation_peak(-1), saturation_mean(-1), saturation_stddev(-1),
        value_peak(-1), value_mode(-1), value_mean(-1), value_stddev(-1),
        locX(-1), locY(-1),
        rectangularity(-1), axisFirst(-1), axisSecond(-1),
        aspectRatio(-1), solidity(-1), massCenterlocX(-1), massCenterlocY(-1),
        arc(-1), radius(-1) {}

    void setInImg(BufferedPort<ImageOf<PixelRgb> > * _pInImg);
    void setOutImg(BufferedPort<ImageOf<PixelRgb> > * _pOutImg);
    void setOutPort(Port *_pOutPort);
    void init(ResourceFinder &rf);
    void run();  // The periodical function
};

#endif  // __SEGMENTOR_THREAD_HPP__

