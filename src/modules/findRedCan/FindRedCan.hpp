// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __FIND_RED_CAN_HPP__
#define __FIND_RED_CAN_HPP__

#include "FinderThread.hpp"

using namespace yarp::os;
using namespace yarp::sig;

class FindRedCan : public RFModule {
  private:
    FinderThread finderThread;
    //
    BufferedPort<ImageOf<PixelRgb> > inImg;
    BufferedPort<ImageOf<PixelRgb> > outImg;
    Port outPort;

    bool interruptModule();
    double getPeriod();
    bool updateModule();

  public:
    bool configure(ResourceFinder &rf);
};

#endif

