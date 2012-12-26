// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup cartesianServer cartesianServer
 *
 * The \ref cartesianServer module creates an instance of CartesianServer to load a controller (default: "cartesianbot") and acts as the server part of a network wrapper for it.
 *
 * @section cartesianServer_legal Legal
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
 * @section cartesianServer_install Installation
 *
 * The module is compiled when ENABLE_cartesianServer is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section cartesianServer_running Running (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] cartesianServer
\endverbatim
 *
 * You should get a window similar to the one depicted on Figure 1.

\image html ravebot-300px.png
<center>Fig. 1 - An instance of the \ref cartesianServer module.</center>

 * @section cartesianServer_interfacing Interfacing with the cartesianServer module
 *
 * The \ref cartesianServer module acts as the server part of a network wrapper of the CartesianBot class
 * using the CartesianServer class.
 * The implementation maps certain YARP rpc's to CartesianBot function calls. Therefore, we can interface
 * with the class from the command-line (or try using the new \ref CartesianClient library) by typing:
\verbatim
[on terminal 3] yarp rpc /ravebot/cartesianServer/rpc:i
\endverbatim
 *
 * Note 1: Change 'ravebot' for 'canbot' for the real robot!
 *
 * Note 2: The default configuration also loads a YARP controlboard device, so you can interact with it as in \ref testRaveBot too.
 *
 * Note 3: Remember that the use of [brackets] means we are sending a VOCAB. The use of (parenthesis) means
 * we are sending a list, which is a Bottle inside a Bottle.
 *
 * We send Cartesian positions/orientations as lists of five elements:
 * <b>x</b>[m], <b>y</b>[m], <b>z</b>[m], <b>rot(y')</b>[deg], <b>rot(z'')</b>[deg] of the end-effector in <i>absolute base coordinates</i>. The first rotation, <b>rot(z)</b>, is given by <b>x</b> and <b>y</b>.

 * The following table depicts implemented RPC commands you can issue from this connection (similar to the CartesianClient class API, as it actually wraps these commands). 
 *
 * <table>
 * <tr class="fragment"><td>rpc command format</td><td>example response</td><td>description</td></tr>
 * <tr><td>[inv] (0.3 0.3 0.7 90 0)</td><td>(45.0 -41.169914 116.855705 14.314209 0.0) [ok]</td><td>Kinematic inversion without movement, returns the joint values that would be needed to reach that position.</td></tr>
 * <tr><td>[movj] (.1 .1 .7 90 0)</td><td>[ok]</td><td>Movement with interpolation in the Joint space.</td></tr>
 * <tr><td>[movl] (.1 .3 .8 90 0)</td><td>[ok]</td><td>Movement with interpolation in Cartesian space.</td></tr>
 * <tr><td>[stat]</td><td>(0.0 0.0 1.4 0.0 0.0) [ok]</td><td>Status poll, returns the current cartesian position (perform direct kinematics).</td></tr>
 * <tr><td>[stop] </td><td>[ok]</td><td>Stop.</td></tr>
 * </table>
 *
 * As an example of use, we can get the current Cartesian position (perform direct kinematics) by sending a <b>stat</b> rpc: 
\verbatim
[on terminal 3] [stat]
\endverbatim
 * And should get some kind of feedback, such as:
\verbatim
Response: (0.0 0.0 1.4 0.0 0.0) [ok]
\endverbatim
 * 
 * The implementation also maps certain YARP streaming commands to CartesianBot function calls. Therefore, we can also interface with the class from the command-line by typing (change 'ravebot' for 'canbot' for the real robot): 
 * 
\verbatim
[on terminal 4] yarp write ... /ravebot/cartesianServer/command:i
\endverbatim
 *
 * The following table depicts implemented streaming commands you can issue from this connection (no acknowledgement response).
 *
 * <table>
 * <tr class="fragment"><td>streaming command format</td><td>description</td></tr>
 * <tr><td>[bkwd] (0.0 90.0)</td><td>[Track virtual point behind the end-effector] (dot(rot(z))[deg/s] rot(y')[deg])</td></tr>
 * <tr><td>[fwd] (0.0 90.0)</td><td>[Track virtual point in front of end-effector] (dot(rot(z))[deg/s] rot(y')[deg]))</td></tr>
 * <tr><td>[rot] (0.0 90.0)</td><td>[Track virtual point orientation] (dot(rot(z))[deg/s] rot(y')[deg]))</td></tr>
 * <tr><td>[vmos] (0.0 1.0 0.0 0.0 0.0)</td><td>Direct velocity movement command, in the Cartesian space.</td></tr>
 * </table>
 *
 * @section cartesianServer_modify Modify
 *
 * This file can be edited at 
 * src/modules/cartesianServer/main.cpp
 *
 */

#include "CartesianServer.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(RlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(RlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("cartesianServer/conf");
    rf.setDefaultConfigFile("cartesianServer.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    CartesianServer mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"cartesianServer --help\" for options.\n");
    printf("cartesianServer checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\ncartesianServer found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}
