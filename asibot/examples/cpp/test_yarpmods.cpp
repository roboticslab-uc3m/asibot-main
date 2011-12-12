// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * \defgroup test_yarpmods
 *
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Copyright: (C) 2010 RobotCub Consortium
 *
 * Author: Juan G Victores
 *
 * Contribs: Paul Fitzpatrick and Giacomo Spigler (YARP dev/motortest.cpp example)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 */

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <stdlib.h>

#include <yarp/dev/PolyDriver.h>

#include <yarp/dev/ControlBoardInterfaces.h>

#include <yarp/dev/CartesianControl.h>
#include <yarp/sig/Vector.h>

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlPlugins);

int main(int argc, char *argv[]) {

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    YARP_REGISTER_PLUGINS(rlPlugins);

    Property options;
    options.put("device","controlboard");
    options.put("subdevice","ravebot");
    options.put("name","/ravebot");
// To connect to a remote "ravebot", comment the last three lines and use:
//    options.put("device","remote_controlboard");
//    options.put("remote","/ravebot");
//    options.put("local","/local");

    PolyDriver dd(options);
    if(!dd.isValid()) {
      printf("ravebot device not available.\n");
	  dd.close();
      Network::fini();
      return 1;
    }

    IPositionControl *pos;
    dd.view(pos);

    int jnts = 0;
    pos->getAxes(&jnts);
    printf("axes: %d\n", jnts);

    Time::delay(5);

    pos->positionMove(1, -45);

    Time::delay(5);

/************** Cartesian Control Part ********************/

    Property cart_options;
    cart_options.put("device","cartesiankontroller");
    PolyDriver cart_dd(cart_options);
    
    if (!cart_dd.isValid()) {
        printf("[test] [error] cartesianKontroller instantiation not worked\n\n");
        printf("ravebot device not available.\n");
        cart_dd.close();
	    dd.close();
        Network::fini();
        return -1;
    }

    ICartesianControl *cart;
    cart_dd.view(cart);

    yarp::sig::Vector myx;
    myx.push_back(0.0);
    myx.push_back(0.0);
    myx.push_back(1.0);

    cart->goToPosition(myx,4.0);

    cart->waitMotionDone(0.1,20.0);
    
    cart_dd.close();
    dd.close();

    return 0;
}


