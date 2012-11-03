// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOBILE_RPC_RESPONDER__
#define __MOBILE_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <openrave-core.h>
#include <openrave/kinbody.h>

#define VOCAB_FAILED VOCAB4('f','a','i','l')
#define VOCAB_OK VOCAB2('o','k')

using namespace yarp::os;
using namespace OpenRAVE;

/**
 * @ingroup MobileRpcResponder
 *
 * MobileRpcResponder class implements a worldRpcServer responder (callback on RPC).
 */
class MobileRpcResponder : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

    EnvironmentBasePtr pEnv;  // set in setEnvironment
    RobotBasePtr pMobile;  // set in setRobot

public:

    /**
    * Register an OpenRAVE environment.
    */
    void setEnvironment(EnvironmentBasePtr _pEnv);

    /**
    * Register an OpenRAVE mobile robot.
    */
    void setMobile(RobotBasePtr _pMobile);
};

#endif  // __MOBILE_RPC_RESPONDER__

