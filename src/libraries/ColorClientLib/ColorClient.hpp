// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __GROUNDING_CLIENT__
#define __GROUNDING_CLIENT__

#include <stdio.h>  // printf, fprintf

#include <vector>

#include <yarp/os/Bottle.h>
#include <yarp/os/ConstString.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/Vocab.h>
#include <yarp/sig/Vector.h>

#define VOCAB_FAILED VOCAB4('f','a','i','l')

using yarp::os::Bottle;
using yarp::os::ConstString;
using yarp::os::RpcClient;

/**
 * @ingroup xgnitive_libraries
 *
 * \defgroup ColorClientLib
 *
 * @brief The ColorClientLib library is composed by a single class, ColorClient.
 * 
 * @section Examples
 * 
 * In C++ (assuming correct installation): \ref testRemoteColor
 *
 * In Python (assuming correct installation with Multi-language Support): \ref testRemoteColorPy
 *
 */

/**
 * @ingroup ColorClient
 *
 * The ColorClient class implements a client part that connects to a remote
 * \ref colorXxx module. Warning: Be careful with NUM_AXES, depends on angle representation!
 * 
 */
class ColorClient {
protected:
    RpcClient rpcClient;
    bool isQuiet;

public:

    /**
     * Constructor.
     */
    ColorClient();

    /**
     * Configure the object and make it connect to a colorXxx module by port name.
     * @param serverPrefix the colorXxx module port name prefix ('/ravebot' or '/canbot').
     * @param quiet suppress messages displayed upon success/failure.
     * @return true if the object and connection was created successfully.
     */
    bool open(const char *serverPrefix, bool quiet=true);

    /**
     * Close the port and object graciously.
     * @return true if the object and connection was closed successfully
     */
    bool close();

    /**
     * Get values of a given feature for a given word.
     * @param word
     * @param feature
     * @param values returned
     * @return true
     */
    bool get(const char *word, const int &feature, std::vector<double> &values);

    /**
     * Get number of features for a given word (does not include timestamp in count).
     * @param word
     * @param size return
     * @return true
     */
    bool size(const char *word, int &value);
};

#endif

