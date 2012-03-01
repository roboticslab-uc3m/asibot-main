// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup cartesianServer cartesianServer
 *
 * The \ref cartesianServer module creates an instance of CartesianBot and acts as the server part of a network wrapper for it.
 *
 * <b> Legal </b>
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses);
 *          Ugo Pattacini (author of <a href="http://eris.liralab.it/iCub/main/dox/html/icub_anyrobot_cartesian_interface.html">Customizing the Cartesian Interface for a Generic Robot</a>, email responses)
 *

 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_cartesianServer is activated (default: OFF). For further
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
 * <b>Interfacing with the \ref cartesianServer module</b>
 *
 * The \ref cartesianServer module acts as the server part of a network wrapper of the CartesianBot class
 * using the CartesianServer class.
 * The implementation maps certain YARP rpc's to CartesianBot function calls. Therefore, we can interface
 * with the class from the command-line by typing:
\verbatim
[on terminal 3] yarp rpc /cartesianServer/rpc:i
\endverbatim
 * We can get the current cartesian position (perform direct kinematics) by sending a <b>stat</b> rpc: 
\verbatim
[on terminal 3] [stat]
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: (0.0 0.0 1.4 0.0 0.0) [ok]
\endverbatim
 * This corresponds to <b>x</b>[m], <b>y</b>[m], <b>z</b>[m], <b>rot(y')</b>[deg], <b>rot(z'')</b>[deg] of the end-effector in <i>absolute base coordinates</i>. The first rotation, <b>rot(z)</b>, is given by <b>x</b> and <b>y</b>.
 *
 * Another implemented behavior is kinematic inversion without movement.
\verbatim
[on terminal 3] [inv] (0.3 0.3 0.7 90 0)
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: (45.0 -41.169914 116.855705 14.314209 0.0) [ok]
\endverbatim
 * Which correspond to the joint values that would be needed to reach that position.
 
 * To actually move the robot, two rpc methods have been implemented: <b>movj</b> and <b>movl</b>
 *
 * Example with <b>movj</b>
\verbatim
[on terminal 3] [movj] (.1 .1 .7 90 0)
\endverbatim
\verbatim
Response: [ok]
\endverbatim
 * Example with <b>movl</b>
\verbatim
[on terminal 3] [movl] (.1 .3 .8 90 0)
\endverbatim
\verbatim
Response: [ok]
\endverbatim
 *
 * Of course, a <b>stop</b> rpc method has been implemented:
\verbatim
[on terminal X] [stop]
\endverbatim
\verbatim
Response: [ok]
\endverbatim
 *
 * <b>Modify</b>
 *
 * This file can be edited at 
 * src/modules/cartesianServer/main.cpp
 *
 */

#include "CartesianServer.h"

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

    CartesianServer mod;
    return mod.runModule(rf);
}

