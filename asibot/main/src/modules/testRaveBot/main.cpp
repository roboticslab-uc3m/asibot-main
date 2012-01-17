// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup testRaveBot
 *
 * Creates an instance of RaveBot.
 *
 * <b> Legal </b>
 *
 * Copyright: 2011 (C) Universidad Carlos III de Madrid; 2006 (C) RobotCub Consortium
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
 * The module is compiled when enable_ASIBOT_MODULES is activated (default). For further
 * installation steps refer to <a href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/testRaveBot
 *
 * <b>Modify</b>
 * 
 * This file can be edited at 
 * src/modules/testRaveBot/main.cpp
 *
 */

#include "testRaveBot.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlPlugins);

int main(int argc, char *argv[]) {

    YARP_REGISTER_PLUGINS(rlPlugins);

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("testRaveBot/conf");
    rf.setDefaultConfigFile("testRaveBot.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("No yarp network, bye!\n");
        return -1;
    }

    testRaveBot mod;
    return mod.runModule(rf);
}

