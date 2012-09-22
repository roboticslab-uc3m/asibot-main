// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WORLD_RPC_RESPONDER__
#define __WORLD_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <openrave-core.h>
#include <openrave/kinbody.h>

#define VOCAB_FAILED VOCAB4('f','a','i','l')
#define VOCAB_OK VOCAB2('o','k')

using namespace yarp::os;
using namespace OpenRAVE;

/**
 * @ingroup WorldRpcResponder
 *
 * WorldRpcResponder class implements a worldRpcServer responder (callback on RPC).
 */
class WorldRpcResponder : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

    KinBodyPtr kinBodyPtr;

    EnvironmentBasePtr pEnv;  // set in setEnvironment
    RobotBasePtr pRobot;  // set in setRobot
public:

    /**
    * Register an OpenRAVE environment.
    */
    void setEnvironment(EnvironmentBasePtr _pEnv);

    /**
    * Register an OpenRAVE robot.
    */
    void setRobot(RobotBasePtr _pRobot);
};

#endif  // __WORLD_RPC_RESPONDER__

