// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_MAKE_UP__
#define __TASK_MAKE_UP__

#include <yarp/os/all.h>

#include "CartesianClient.h"

using namespace yarp::os;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TaskMakeUpLib
 *
 * The \ref TaskMakeUpLib library is composed by a single class, TaskMakeUp.
 *
 */

/**
 * @ingroup TaskMakeUp
 *
 * The TaskMakeUp class implements a client part that connects to a remote
 * \ref cartesianServer module. Warning: Be careful with NUM_AXES, depends on angle representation!
 * 
 */
class TaskMakeUp {
protected:
    CartesianClient cartesianClient;
    
    public:

    /**
     * Constructor.
     */
    TaskMakeUp();

    /**
     * Configure the object and make it connect to a cartesianServer module by port name.
     * @param serverPrefix the cartesianServer module port name prefix ('/ravebot' or '/canbot').
     * @return true if the object and connection was created successfully
     */
    bool open(const ConstString& serverPrefix);

    /**
     * Close the port and object graciously.
     * @return true if the object and connection was closed successfully
     */
    bool close();

    /**
     * Run.
     * @return true/false on success/failure.
     */
    bool run(const ConstString a, const ConstString b, const ConstString c, const ConstString d, const ConstString e, const ConstString f, const ConstString g);
};

#endif  // __TASK_CATCH_TOOL__

