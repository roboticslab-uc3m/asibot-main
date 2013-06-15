// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __VISION_DEPTH_HPP__
#define __VISION_DEPTH_HPP__

#include "SegmentorThread.hpp"

#define DEFAULT_WATCHDOG    2       // [s]

using namespace yarp::os;
using namespace yarp::sig;

class VisionDepth : public RFModule {
  private:
    SegmentorThread segmentorThread;
    //
    BufferedPort<ImageOf<PixelFloat> > inDepth;
    BufferedPort<ImageOf<PixelRgb> > inImg;
    BufferedPort<ImageOf<PixelRgb> > outImg;
    Port outPort;

    bool interruptModule();
    double getPeriod();
    bool updateModule();
    double watchdog;

  public:
    bool configure(ResourceFinder &rf);
};

#endif  // __VISION_DEPTH_HPP__

