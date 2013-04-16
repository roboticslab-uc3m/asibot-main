// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup webInterface webInterface
 *
 * @brief Creates an instance of the WebInterface class. Calls are handled by the WebResponder class.
 * It is used by the \ref asibot_webInterface.
 *
 * \image html webInterface.png 
 * <center>Fig. 1 - Web browser screenshot of contents provided by the \ref webInterface module (see 
 * <a href="http://youtu.be/9B0tVJsg-0I" target="_blank">webInterface Video</a>).</center>
 *
 * <b> Legal </b>
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_webInterface is activated (default: OFF). For further
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
[on terminal 2] $ASIBOT_DIR/bin/webInterface
\endverbatim
 * And should get some kind of feedback. The last line should be something like:
\verbatim
Server running, visit: http://127.0.0.1:8080/index
\endverbatim
 *
 * Now open a browser and go to the location indicated by that feedback, as in:
 *
\verbatim
[on terminal 3] firefox http://127.0.0.1:8080/index &
\endverbatim
 *
 * <b>Modify</b>

 * This file can be edited at 
 * src/modules/webInterface/main.cpp
 *
 */

#include "WebInterface.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(RlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(RlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("webInterface/conf");
    rf.setDefaultConfigFile("webInterface.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    WebInterface mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"webInterface --help\" for options.\n");
    printf("webInterface checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\nwebInterface found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

