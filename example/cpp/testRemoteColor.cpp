// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * @ingroup asibot_examples_cpp
 *
 * \defgroup testRemoteColor testRemoteColor
 *
 * @brief This example connects to a running \ref colorSegmentor or \ref colorDepth module using the C++ implementation
 * of the \ref ColorClientLib library.
 *
 * @section testRemoteColor_legal Legal
 *
 * Copyright: (C) 2013 Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * @section testRemoteColor_build Building
\verbatim
cd $ASIBOT_ROOT/example/cpp
mkdir build; cd build; cmake ..
make -j3
\endverbatim
 *
 * @section testRemoteColor_run Running
\verbatim
./testRemoteColor
\endverbatim
 *
 */

#include <stdio.h>

#include <vector>

#include <yarp/os/Network.h>

#include "ColorClient.hpp"

using yarp::os::Network;

using std::vector;

int main(int argc, char *argv[]) {

    Network yarp;  // connect to YARP network

    if (!Network::checkNetwork()) {  // let's see if there was actually a reachable YARP network
        fprintf(stderr,"[fail] %s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return(-1);
    }

    ColorClient color;  // create an instance of ColorClient
    //if (!color.open("/colorSegmentor",false)) {  // use this line instead of next one for debuging purposes.
    if (!color.open("/colorSegmentor")) {  // tries to connect. syntax: grounding.open("/prefix",quiet=true);
        printf("[fail] Could not open ColorClient. Bye!\n");
        return(-1);
    }

    bool ok = true;

    int size;
    ok &= color.size(size);  // Get number of coordinates per point, put in size.

    vector<double> values;
    ok &= color.get(values);  // Get coordinates, put in values.

    if (!ok) printf("[warning] Something failed!\n");

    printf("Number of coordinates per point (should remain static): %d\n",size);

    printf("Values:");
    for (int i=0; i<values.size(); i++)
        printf(" %f",values[i]);
    printf("\n");

    color.close();

    printf("Bye!\n");

    return 0;
}


