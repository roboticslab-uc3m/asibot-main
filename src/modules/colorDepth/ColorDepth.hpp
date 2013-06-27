// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __COLOR_SEGMENTOR_HPP__
#define __COLOR_SEGMENTOR_HPP__

#include "SegmentorThread.hpp"

#define DEFAULT_WATCHDOG    2       // [s]

using namespace yarp::os;
using namespace yarp::sig;

class ColorDepth : public RFModule {
  private:
    SegmentorThread segmentorThread;
    //
    PolyDriver dd;
    IKinectDeviceDriver *kinect;
    
    BufferedPort<ImageOf<PixelRgb> > outImg;
    Port outPort;

    bool interruptModule();
    double getPeriod();
    bool updateModule();
    double watchdog;

  public:
    bool configure(ResourceFinder &rf);
};

#endif

