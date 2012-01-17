// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_SERVER__
#define __CARTESIAN_SERVER__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
//#include <yarp/dev/CartesianControl.h>

#define DEFAULT_ROBOT_NAME "ravebot"

using namespace yarp::os;
using namespace yarp::dev;

class testRaveBot : public RFModule {
protected:
    yarp::dev::PolyDriver robotDevice;

//	qSetPort qPort;  // to connect to robot arm, encoders
//	mSetPort mPort;  // to set its mode from outside
//    sharedArea mem;

//    WBCthread wThread;

//    double getPeriod();
    bool updateModule();
//    bool interruptModule();
//    int period;

public:
    testRaveBot();
    bool configure(ResourceFinder &rf);
};

#endif

