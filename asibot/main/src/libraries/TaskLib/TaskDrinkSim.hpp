// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_DRINK_SIM_HPP__
#define __TASK_DRINK_SIM_HPP__

#include "TaskBase.hpp"

#include <yarp/os/RpcClient.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/SVD.h>  // pinv
//#include <yarp/math/Math.h>

#include "CartesianClient.h"
#include "TinyMath.hpp"

#define DEFAULT_ROBOT_ANGLE 30.0

/**
 * @ingroup TaskDrinkSim
 *
 * The TaskDrinkSim class implements a client part that connects to a remote
 * \ref cartesianServer module.
 * 
 */
class TaskDrinkSim : TaskBase {
  protected:
    CartesianClient _cartesianClient;
    RpcClient _worldRpcClient;
    double _robotAngle;

  public:
    virtual bool configure(yarp::os::Searchable& config);
    virtual bool run();
    virtual bool close();
};

#endif  // __TASK_DRINK_SIM_HPP__

