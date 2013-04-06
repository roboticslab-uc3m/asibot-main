// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __GROUNDING_CLIENT__
#define __GROUNDING_CLIENT__

#include <stdio.h>  // printf, fprintf

#include <vector>

#include <yarp/os/Bottle.h>
#include <yarp/os/ConstString.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Vocab.h>
#include <yarp/os/Network.h>
#include <yarp/sig/Vector.h>

#define VOCAB_FAILED VOCAB4('f','a','i','l')

using yarp::os::Bottle;
using yarp::os::BufferedPort;
using yarp::os::ConstString;
using yarp::os::Network;

/**
 * @ingroup asibot_libraries
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
 * \ref colorSegmentor or \ref colorDepth module.
 * 
 */
class ColorClient {
protected:
    BufferedPort<Bottle> stateClient;
    bool isQuiet;

public:

    /**
     * Constructor.
     */
    ColorClient();

    /**
     * Configure the object and make it connect to a colorXxx module by port name.
     * @param serverPrefix the colorXxx module port name prefix ('/colorSegmentor' or '/colorDepth').
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
     * @param index
     * @param values returned
     * @return true
     */
    bool get(const int &index, std::vector<double> &values);

    /**
     * Get number for maximum index.
     * @param size return
     * @return true
     */
    bool size(int &value);
};

#endif

