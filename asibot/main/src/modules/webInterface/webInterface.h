// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_INTERFACE__
#define __WEB_INTERFACE__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include "webResponder.h"

using namespace yarp::os;

class WebInterface : public RFModule {
protected:

    Port server;
    WebResponder responder;
    Contact contact;

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

