// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TEST_RAVEBOT__
#define __TEST_RAVEBOT__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>

#define DEFAULT_ROBOT_NAME "ravebot"

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TestRaveBotLib
 *
 * The \ref TestRaveBotLib library is composed by a single class, TestRaveBot.
 *
 */

/**
 * @ingroup TestRaveBot
 *
 * The TestRaveBot class tests the RaveBot class as a controlboard.
 * 
 */
class TestRaveBot : public RFModule {
protected:
    yarp::dev::PolyDriver robotDevice;

//    double getPeriod();
    bool updateModule();
//    bool interruptModule();
//    int period;

public:
    TestRaveBot();
    bool configure(ResourceFinder &rf);
};

#endif

