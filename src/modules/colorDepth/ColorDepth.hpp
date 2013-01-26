// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __COLOR_SEGMENTOR_HPP__
#define __COLOR_SEGMENTOR_HPP__

#include "SegmentorThread.hpp"

#define DEFAULT_FX          640     //
#define DEFAULT_FY          640     //
#define DEFAULT_CX          320     //
#define DEFAULT_CY          240     //
#define DEFAULT_WATCHDOG    2       // [s]

using namespace yarp::os;
using namespace yarp::sig;

class ColorDepth : public RFModule {
  private:
    SegmentorThread segmentorThread;
    //
    BufferedPort<ImageOf<PixelFloat> > depthPort;
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

#endif

