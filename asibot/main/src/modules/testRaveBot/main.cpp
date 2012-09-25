// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup testRaveBot testRaveBot
 *
 * Creates an instance of RaveBot wrapped in a YARP controlboard. This means that it may be used
 * through a YARP remote_controlboard or directly through low-level YARP controlboard RPC commands.
 *
 * <b> Legal </b>
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses);
 *          Rosen Diankov (OpenRAVE sample code, email responses)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_testRaveBot is activated (default: OFF). For further
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
[on terminal 2] $ASIBOT_DIR/bin/testRaveBot
\endverbatim
 *
 * <b>Interfacing with the testRaveBot</b>
 *
 * The \ref testRaveBot module acts as the server part of a network wrapper of the RaveBot class.
 * The implementation maps certain YARP rpc's to RaveBot function calls. Therefore, we can interface
 * with the class from the command-line by typing:
\verbatim
[on terminal 3] yarp rpc /ravebot/rpc:i
\endverbatim
 * We can send an absolute position joint space movement such as: 
\verbatim
[on terminal 3] set poss (45.0 -80.455885 110.552447 59.903438 0.0)
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: [ok]
\endverbatim
 *
 * The RaveBot class also attaches a WorldRpcResponder that provides certain interaction with the
 * environment. We can interface with the class from the command-line by typing:
\verbatim
[on terminal 4] yarp rpc /ravebot/world
\endverbatim
 * It is possible to create boxes. Example of a .1m^3 box situated at 1 meter Z:
\verbatim
[on terminal 4] world mk box .1 .1 .1 0 0 1
\endverbatim
 * Grab last created box (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab 1
\endverbatim
 * Release last created box (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab 0
\endverbatim
 *
 * The RaveBot class additionally streams the data of the last camera sensor sensor it finds. We can
 * view this data from the command-line by typing:
\verbatim
[on terminal 5] yarpview /yarpview/img:i
[on terminal 5] yarp connect /ravebot/img:o /yarpview/img:i 
\endverbatim
 * 
 * The RaveBot class also streams the data of the last depth sensor sensor it finds. We can
 * view this data from the command-line by typing:
\verbatim
[on terminal 6] yarpview /yarpview/depth:i
[on terminal 6] yarp connect /ravebot/depth:o /yarpview/depth:i 
\endverbatim
 * 
 *
 * <b>Modify</b>
 * 
 * This file can be edited at 
 * $ASIBOT_DIR/main/src/modules/testRaveBot/main.cpp
 *
 */

#include "TestRaveBot.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(RlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(RlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("testRaveBot/conf");
    rf.setDefaultConfigFile("testRaveBot.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    TestRaveBot mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"testRaveBot --help\" for options.\n");
    printf("testRaveBot checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\ntestRaveBot found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

