// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * @ingroup asibot_examples_cpp
 *
 * \defgroup testRemoteCartesianServer testRemoteCartesianServer
 *
 * @brief This example connects to a running \ref cartesianServer module using the C++ implementation
 * of the \ref CartesianClientLib library.
 *
 * <b>Legal</b>
 *
 * Copyright: (C) 2012 Universidad Carlos III de Madrid
 *
 * Author: Juan G Victores
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Building</b>
\verbatim
cd $ASIBOT_ROOT/example/cpp
mkdir build; cd build; cmake ..
make -j3
\endverbatim
 *
 * <b>Running</b>
\verbatim
./testRemoteCartesianServer
\endverbatim
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "CartesianClient.h"

using namespace yarp::os;
using namespace yarp::dev;

int main(int argc, char *argv[]) {

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    CartesianClient simCart;
    simCart.open("/ravebot");
    
    double targets[5] = {0.1, 0.0, 0.9, 90.0, 0.0};
    printf("Movj to targets: {0.1, 0.0, 0.9, 90.0, 0.0}...\n");
    simCart.movj(targets);

    printf("Delaying 5 seconds...\n");
    Time::delay(5);

    printf("Stopping...\n");
    simCart.stop();

    double newtargets[5] = {0.4, 0.0, 0.9, 90.0, 0.0};
    printf("Movl to newtargets: {0.4, 0.0, 0.9, 90.0, 0.0}...\n");
    simCart.movl(newtargets);

    printf("Delaying 20 seconds...\n");
    Time::delay(20);

    double cartCoords[5];
    simCart.stat(cartCoords);
    printf("At: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);

    printf("Bye!\n");
    simCart.close();

    return 0;
}


