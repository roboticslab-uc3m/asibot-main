// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup asibot_modules
 * \defgroup testRaveBot testRaveBot
 *
 * Creates an instance of RaveBot wrapped in a YARP controlboard. This means that it may be used
 * through a YARP remote_controlboard or directly through low-level YARP controlboard RPC commands.
 *
 * @section testRaveBot_legal Legal
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
 * @section testRaveBot_install Installation
 *
 * The module is compiled when ENABLE_testRaveBot is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section testRaveBot_running Running (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] testRaveBot
\endverbatim
 *
 * @section testRaveBot_interfacing Interfacing with the testRaveBot module
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
 * It is possible to create static (not affected by gravity) boxes. Example of a .1m^3 static box situated at 1 meter X, 2 meters Y, 1.5 meter Z:
\verbatim
[on terminal 4] world mk sbox .1 .1 .1 1 2 1.5
\endverbatim
 * Grab the first created static box (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab sbox 1 1
\endverbatim
 * Release the first created static box (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab sbox 1 0
\endverbatim
 *
 * It is possible to create static (not affected by gravity) spheres. Example of a static shpere of .05 meter radius, situated at 2 meters X, 1.5 meter Y, 1 meter Z:
\verbatim
[on terminal 4] world mk ssph .05 2 1.5 1
\endverbatim
 * Grab the first created static sphere (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab ssph 1 1
\endverbatim
 * Release the first created static sphere (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab ssph 1 0
\endverbatim
 *
 * It is possible to create static (not affected by gravity) cylinders. Example of a static cylinder of .05 meter radius and 1 meter height, situated at 1.2 meters X, 1.3 meter Y, 1.4 meter Z:
\verbatim
[on terminal 4] world mk scyl .05 1 1.2 1.3 1.4
\endverbatim
 * Grab the first created static cylinder (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab scyl 1 1
\endverbatim
 * Release the first created static cylinder (no fingers needed, just pure magic!):
\verbatim
[on terminal 4] world grab scyl 1 0
\endverbatim
 *
 * Delete all of the created objects with the following command:
\verbatim
[on terminal 4] world del all
\endverbatim
 *
 * <b>New!</b> You can even grab pre-existent objects of the environment. For
 * example, in the ASIBOT kitchen environment:
\verbatim
[on terminal 4] world grab obj redCan 1
\endverbatim
 * Release the pre-existent objects of the environment changing the last parameter:
\verbatim
[on terminal 4] world grab obj redCan 0
\endverbatim
 *
 * The RaveBot class additionally streams the data of every camera and depth
 * sensor it finds. Let's say you invoked \ref testRaveBot or \ref
 * cartesianServer with the <b>--env asibot_kitchen_cameras.env.xml</b> as
 * described in \ref post_install_changing_parameters. We can view the ip camera
 * image stream with the standard yarp viewer (make sure CREATE_GUIS was enabled
 * when compiling YARP to have <i>yarpview</i>):
\verbatim
[on terminal 5] yarpview /yarpview/img:i &
[on terminal 5] yarp connect /ravebot/ip_camera/img:o /yarpview/img:i 
\endverbatim
 * 
 *
 * @section testRaveBot_modify Modify
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
    rf.setDefaultContext("testRaveBot");
    rf.setDefaultConfigFile("testRaveBot.ini");
    rf.configure(argc, argv);

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

