// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_INTERFACE__
#define __WEB_INTERFACE__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "webResponder.h"

using namespace yarp::os;
using namespace yarp::dev;

class WebInterface : public RFModule {
protected:

    Port server;
    WebResponder responder;
    Contact contact;

    yarp::dev::PolyDriver cartesianDevice;
    yarp::dev::PolyDriver robotDevice;

    yarp::dev::ICartesianControl *icart;
    yarp::dev::IPositionControl *ipos;

    bool updateModule();
    bool interruptModule();
    double getPeriod();
    double period;

    int counter;

public:
    WebInterface();
    bool configure(ResourceFinder &rf);
};

#endif

