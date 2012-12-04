// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __PAN_TILT_RPC_RESPONDER__
#define __PAN_TILT_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>

#include <openrave-core.h>
#include <openrave/kinbody.h>

#define VOCAB_FAILED VOCAB4('f','a','i','l')
#define VOCAB_OK VOCAB2('o','k')
#define VOCAB_IS VOCAB2('i','s')
#define VOCAB_SET VOCAB3('s','e','t')
#define VOCAB_GET VOCAB3('g','e','t')
#define VOCAB_POSS VOCAB('p','o','s','s')
#define VOCAB_RELS VOCAB('r','e','l','s')

using namespace yarp::os;
using namespace OpenRAVE;

/**
 * @ingroup PanTiltRpcResponder
 *
 * PanTiltRpcResponder class implements a panTiltRpcServer responder (callback on RPC).
 */
class PanTiltRpcResponder : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

    EnvironmentBasePtr pEnv;  // set in setEnvironment
    RobotBasePtr pPanTilt;  // set in setRobot

    Semaphore encRawMutex;  // SharedArea
    OpenRAVE::Vector encQuat;

public:

    /**
    * Register an OpenRAVE environment.
    */
    void setEnvironment(EnvironmentBasePtr _pEnv);

    /**
    * Register an OpenRAVE panTilt robot.
    */
    void setPanTilt(RobotBasePtr _pPanTilt);

    void setEncRaw(const int Index, const double Position);
    double getEncRaw(const int Index);
    double getEncExposed(const int Index);

};

#endif  // __PAN_TILT_RPC_RESPONDER__

