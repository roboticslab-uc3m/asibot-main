// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_SERVER__
#define __CARTESIAN_SERVER__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include "TaskRpcResponder.hpp"

#define DEFAULT_CONTROLLER "cartesianbot"
#define DEFAULT_PREFIX "/ravebot"
#define DEFAULT_MOVJ_LOCAL "/ravebot/movjTaskServer"
#define DEFAULT_MOVJ_REMOTE "/ravebot"

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TaskServerLib
 *
 * The \ref TaskServerLib library is composed by a single class, TaskServer.
 *
 */

/**
 * @ingroup TaskServer
 *
 * The TaskServer class implements a server part that receives a connection from a remote
 * \ref cartesianServer module.
 * 
 */
class TaskServer : public RFModule {
protected:

    int *csStatus;

    RpcServer taskRpcServer;
	TaskRpcResponder taskResponder;

    bool updateModule();
    bool interruptModule();
    // double getPeriod();

public:
    TaskServer();
    bool configure(ResourceFinder &rf);
};

#endif

