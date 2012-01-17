// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_SERVER__
#define __CARTESIAN_SERVER__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>

#define DEFAULT_CONTROLLER "cartesianbot"

using namespace yarp::os;
using namespace yarp::dev;

class xCommPort : public BufferedPort<Bottle> {
protected:
    virtual void onRead(Bottle& v) {
        printf("[Debug] Data arrived on xCommPort\n");
    }
};

class cartesianServer : public RFModule {
protected:
    yarp::dev::PolyDriver cartesianDevice;

    yarp::dev::ICartesianControl *icart;

	xCommPort xPort;

    bool updateModule();
    bool interruptModule();
//    double getPeriod();
//    int period;

public:
    cartesianServer();
    bool configure(ResourceFinder &rf);
};

#endif

