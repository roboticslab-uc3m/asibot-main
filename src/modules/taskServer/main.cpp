// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * @ingroup asibot_modules
 * \defgroup taskServer taskServer
 *
 * The \ref taskServer module.
 *
 * @section taskServer_legal Legal
 *
 * Copyright: 2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * @section taskServer_install Installation
 *
 * The module is compiled when ENABLE_taskServer is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * @section taskServer_running Running (assuming correct installation)
 *
 * First we must run a YARP name server if it is not running in our current namespace:
\verbatim
[on terminal 1] yarp server
\endverbatim
 * And then launch the actual module:
\verbatim
[on terminal 2] taskServer
\endverbatim
 *
 * @section taskServer_modify Modify
 *
 * This file can be edited at 
 * src/modules/taskServer/main.cpp
 *
 */

#include "TaskServer.hpp"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(RlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(RlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("taskServer/conf");
    rf.setDefaultConfigFile("taskServer.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    CartesianServer mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"taskServer --help\" for options.\n");
    printf("taskServer checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\ntaskServer found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

