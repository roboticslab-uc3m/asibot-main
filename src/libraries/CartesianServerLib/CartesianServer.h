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
#include <yarp/dev/ControlBoardInterfaces.h>

#include "xRpcResponder.h"
#include "xCallbackPort.h"

#define DEFAULT_CONTROLLER "cartesianbot"
#define DEFAULT_PREFIX "/ravebot"
#define DEFAULT_MOVJ_LOCAL "/ravebot/movjCartesianServer"
#define DEFAULT_MOVJ_REMOTE "/ravebot"

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup CartesianServerLib
 *
 * The \ref CartesianServerLib library is composed by a single class, CartesianServer.
 *
 */

/**
 * @ingroup CartesianServer
 *
 * The CartesianServer class implements a server part that receives a connection from a remote
 * \ref cartesianServer module.
 * 
 */
class CartesianServer : public RFModule {
protected:
    yarp::dev::PolyDriver cartesianDevice;
    yarp::dev::PolyDriver robotDevice;

    yarp::dev::ICartesianControl *icart;
    yarp::dev::IPositionControl *ipos;
    yarp::dev::IControlMode *imode;
    int *csStatus;

    RpcServer xRpcServer;
	xRpcResponder xResponder;

    xCallbackPort xPort;

    bool updateModule();
    bool interruptModule();
    // double getPeriod();

public:
    CartesianServer();
    bool configure(ResourceFinder &rf);
};

#endif

