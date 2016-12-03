// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_CLIENT__
#define __CARTESIAN_CLIENT__

#include <yarp/os/all.h>

#define NUM_AXES 5
#define VOCAB_MY_STOP VOCAB4('s','t','o','p')
#define VOCAB_STAT VOCAB4('s','t','a','t')
#define VOCAB_MOVL VOCAB4('m','o','v','l')
#define VOCAB_MOVJ VOCAB4('m','o','v','j')
#define VOCAB_INV VOCAB3('i','n','v')
#define VOCAB_WAIT VOCAB4('w','a','i','t')

using namespace yarp::os;

/**
 * @ingroup asibot_libraries
 *
 * \defgroup CartesianClientLib
 *
 * The \ref CartesianClientLib library is composed by a single class, CartesianClient. You can
 * find its Python equivalent in repos/asibot-main/example/python/CartesianClient.py
 *
 * @section Examples
 * 
 * In C++ (assuming correct installation): \ref testRemoteCartesianServer
 *
 * In MATLAB (assuming correct installation with Multi-language Support): \ref testRemoteCartesianServerM
 *
 * In Python (assuming correct installation with Multi-language Support): \ref testRemoteCartesianServerPy
 *
 */

/**
 * @ingroup CartesianClient
 *
 * The CartesianClient class implements a client part that connects to a remote
 * \ref cartesianServer module. Warning: Be careful with NUM_AXES, depends on angle representation!
 * 
 */
class CartesianClient {
protected:
    RpcClient rpcClient;
    bool validClient;
public:

    /**
     * Constructor.
     */
    CartesianClient();

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
     * Obtain the current position and orientation.
     * [wait for reply] 
     * @param xd returns with the array of position/orientation doubles.
     * @return true/false on success/failure.
     */
    bool stat(double *xd);

    /**
     * Ask for inverting a given pose without actually moving there.
     * [wait for reply] 
     * @param xd array of doubles with target position/orientation.
     * @param q returns with the array of doubles of joint values.
     * @return true/false on success/failure.
     */
    bool inv(const double *xd, double *q);

    /**
     * Move to absolute position, interpolation at joint level.
     * @param xd array of doubles with target position/orientation.
     * @return true/false on success/failure.
     */
    bool movj(const double *xd);
    
    /**
     * Move to absolute position, interpolation in Cartesian space.
     * @param xd array of doubles with target position/orientation.
     * @return true/false on success/failure.
     */
    bool movl(const double *xd);

    /**
     * Just wait for a movj or movl to be done. TODO: timeinterval, maxwait.
     * @return true/false on success/failure.
     */
    bool wait();

    /**
     * Stop.
     * @return true/false on success/failure.
     */
    bool stop();
};

#endif

