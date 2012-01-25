// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup cartesianServer
 *
 * Creates an instance of CartesianBot and acts as the server part of a network wrapper for it.
 *
 * <b> Legal </b>
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when enable_ASIBOT_MODULES is activated (default). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] $ASIBOT_DIR/bin/cartesianServer
\endverbatim
 *
 * <b>Interfacing with the CartesianServer</b>
 *
 * The CartesianServer module acts as the server part of a network wrapper of the CartesianBot class.
 * The implementation maps certain YARP rpc's to CartesianBot function calls. Therefore, we can interface
 * with the class from the command-line by typing:
\verbatim
[on terminal 3] yarp rpc /cartesianServer/rpc:i
\endverbatim
 * We can get the current cartesian position (perform direct kinematics) by sending a 0: 
\verbatim
[on terminal 3] 0
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: (0.0 0.0 1.4 0.0 0.0) [ok]
\endverbatim
 * This corresponds to <b>x</b>, <b>y</b>, and <b>z</b> in <u>absolute base coordinates</u>, and <b>pitch</b> (angle on y) and <b>roll</b> (angle on z) defined <u>on the end-effector frame</u>.
 *
 * Another implemented behavior is kinematic inversion without movement.
\verbatim
[on terminal 3] 15 (0.3 0.3 0.7 90 0)
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: (45.0 -80.455885 110.552447 59.903438 0.0) [ok]
\endverbatim
 * Which correspond to the joint values that would be needed to reach that position. If you want to actually move
 * the motors to those joint values, remember that CartesianServer instanciates RaveBot as a YARP controlboard,
 * so you can interface with it as with <a class="el" href="group__testRaveBot.html">TestRaveBot</a> as well!
 *
 * <b>Modify</b>

 * This file can be edited at 
 * src/modules/cartesianServer/main.cpp
 *
 */

#include "cartesianServer.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(rlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("cartesianServer/conf");
    rf.setDefaultConfigFile("cartesianServer.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    if (rf.check("help")) {
        fprintf(stdout,"Options:\n");
        fprintf(stdout,"\t--rate    rate: ms (default: \"20\")\n");
        return 0;
    }

    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("No yarp network, bye!\n");
        return -1;
    }

    cartesianServer mod;
    return mod.runModule(rf);
}

