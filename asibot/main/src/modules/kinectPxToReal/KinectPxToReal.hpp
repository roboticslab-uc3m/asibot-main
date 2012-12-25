// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
  * Copyright 2012 UC3M
  * Author: Juan G. Victores
  */

#ifndef __KINECT_PX_TO_REAL_HPP__
#define __KINECT_PX_TO_REAL_HPP__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/sig/Matrix.h>
#include <yarp/sig/all.h>

#include "SharedArea.hpp"
#include "OutThread.hpp"

#define DEFAULT_WATCHDOG    5       // [s]

using namespace yarp::os;
using namespace yarp::sig;

class KinectPxToReal : public RFModule {
    protected:
        bool updateModule();
        bool interruptModule();
        double getPeriod();
        double watchdog; // [s]

        SharedArea *pMem;
        OutThread outThread;

        BufferedPort<ImageOf<PixelFloat> > inDepth;
        Port inPort;
        Port outPort;

    public:
        bool configure(ResourceFinder &rf);
};

#endif  // __KINECT_PX_TO_REAL_HPP__

