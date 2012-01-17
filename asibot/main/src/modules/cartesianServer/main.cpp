// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup cartesianServer
 *
 * Creates an instance of CartesianBot.
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
 * installation steps refer to <a href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/cartesianServer
 *
 * <b>Modify</b>

 * This file can be edited at 
 * src/modules/cartesianServer/main.cpp
 *
 */

#include "cartesianServer.h"

using namespace yarp::os;
using namespace yarp::dev;

//YARP_DECLARE_PLUGINS(rlPlugins);

int main(int argc, char *argv[]) {

    //YARP_REGISTER_PLUGINS(rlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("cartesianServer/conf");
    rf.setDefaultConfigFile("cartesianServer.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("No yarp network, bye!\n");
        return -1;
    }

    cartesianServer mod;
    return mod.runModule(rf);
}

