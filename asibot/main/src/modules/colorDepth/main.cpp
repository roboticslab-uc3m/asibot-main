// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup colorDepth colorDepth
 *
 * \image html colorDepthSim-500px.png
 * 
 * The \ref colorDepth module tracks a red can and streams its location in the format "(x y z) ..." through a
 * <a href="http://eris.liralab.it/yarpdoc/classyarp_1_1os_1_1Port.html">yarp::os::Port</a> output called
 * <b>[prefix]/state:o</b>. The current implementation uses blobs and OpenCV.
 * yarpdev --device opencv_grabber --movie http://CAMERA_IP/mjpg/video.mjpg
 *
 * <b> Legal </b>
 *
 * Copyright: 2013 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_colorDepth is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/colorDepth
 *
 * <b>Modify</b>
 *
 * This file can be edited at src/modules/colorDepth/main.cpp
 *
 */

#include "ColorDepth.hpp"

int main(int argc, char** argv) {

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("colorDepth/conf");
    rf.setDefaultConfigFile("colorDepth.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    ColorDepth mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"%s --help\" for options.\n",argv[0]);
    printf("%s checking for yarp network... ",argv[0]);
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        fprintf(stderr,"[fail]\n%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

