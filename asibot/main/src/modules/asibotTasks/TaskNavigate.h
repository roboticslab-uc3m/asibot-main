// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_NAVIGATE__
#define __TASK_NAVIGATE__

#include <yarp/os/all.h>

#define VOCAB_SET VOCAB3('s','e','t')
#define VOCAB_POSS VOCAB('p','o','s','s')

using namespace yarp::os;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TaskNavigateLib
 *
 * The \ref TaskNavigateLib library is composed by a single class, TaskNavigate.
 *
 */

/**
 * @ingroup TaskNavigate
 *
 * The TaskNavigate class implements a client part that connects to a remote
 * \ref cartesianServer module. Warning: Be careful with NUM_AXES, depends on angle representation!
 * 
 */
class TaskNavigate {
protected:
    RpcClient rpcClient;
    bool validClient;
public:

    /**
     * Constructor.
     */
    TaskNavigate();

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
    bool run(const ConstString a, const ConstString b, const ConstString c, const ConstString d);
};

#endif  // __TASK_NAVIGATE__

