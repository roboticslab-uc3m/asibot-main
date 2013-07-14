// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_GRAB_CAN_SIM_HPP__
#define __TASK_GRAB_CAN_SIM_HPP__

#include "TaskBase.hpp"

#include "CartesianClient.h"
#include <yarp/os/RpcClient.h>

#define DEFAULT_ROBOT_ANGLE 30.0

/**
 * @ingroup TaskGrabCanSim
 *
 * The TaskGrabCanSim class implements a client part that connects to a remote
 * \ref cartesianServer module.
 * 
 */
class TaskGrabCanSim : TaskBase {
  protected:
    CartesianClient _cartesianClient;
    RpcClient _worldRpcClient;
    double _robotAngle;

  public:
    virtual bool configure(yarp::os::Searchable& config);
    virtual bool run();
    virtual bool close();
};

#endif  // __TASK_GRAB_CAN_SIM_HPP__

