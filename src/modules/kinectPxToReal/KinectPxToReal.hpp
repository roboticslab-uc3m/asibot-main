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

#define DEFAULT_WATCHDOG    5       // [s]

using namespace yarp::os;

class PremultH : public RFModule {
    protected:
        bool updateModule();
        bool interruptModule();
        double getPeriod();
        double watchdog; // [s]

        Port outPort;
        PremultPorts premultPorts;

        /**
        * [thanks Ugo Pattacini!]
        */
        bool getMatrixFromProperties(Searchable &options, ConstString &tag, yarp::sig::Matrix &H);
        yarp::sig::Matrix H;

    public:
        bool configure(ResourceFinder &rf);
};

#endif  // __KINECT_PX_TO_REAL_HPP__

