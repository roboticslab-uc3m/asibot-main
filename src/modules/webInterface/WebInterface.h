// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_INTERFACE__
#define __WEB_INTERFACE__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Contact.h>

#include "WebResponder.h"

#define DEFAULT_PERIOD 5   // [s]
#define DEFAULT_RESOURCES "robots.uc3m.es/webInterface/html"
#define DEFAULT_WEB_IP "localhost"
#define DEFAULT_WEB_PORT 8080

using namespace yarp::os;

class WebInterface : public RFModule {
protected:

    Port server;
    WebResponder responder;
    Contact contact;  // will get webIp and webPort

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

