// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_SLIDE_PALLETE__
#define __TASK_SLIDE_PALLETE__

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

using namespace yarp::os;
using namespace yarp::dev;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TaskSlidePalleteLib
 *
 * The \ref TaskSlidePalleteLib library is composed by a single class, TaskSlidePallete.
 *
 */

/**
 * @ingroup TaskSlidePallete
 *
 * The TaskSlidePallete class implements a client part that connects to a remote
 * \ref cartesianServer module. Warning: Be careful with NUM_AXES, depends on angle representation!
 * 
 */
class TaskSlidePallete {
protected:
    IPositionControl *pos;
    IControlMode *mode;
    PolyDriver dd;
    
    public:

    /**
     * Constructor.
     */
    TaskSlidePallete();

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
    bool run(const ConstString a, const ConstString b, const ConstString c, const ConstString d, const ConstString e, const ConstString f);
};

#endif  // __TASK_SLIDE_PALLETE__

