// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup findRedCan findRedCan
 *
 * \image html findRedCan.png
 * 
 * The \ref findRedCan module tracks a red can and streams its location in the format "lata roja [cm] [cm]" through a <a href="http://eris.liralab.it/yarpdoc/classyarp_1_1os_1_1Port.html">yarp::os::Port</a> output called <b>/redCan:o</b>. The current implementation uses blobs and OpenCV, so a cvNamedWindow with the tracking is simultaneously displayed. yarpdev --device opencv_grabber --movie http://CAMERA_IP/mjpg/video.mjpg
 *
 * <b> Legal </b>
 *
 * Copyright: 2011 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Carlos Fernández-Portal (author of the MATLAB code that contained the original algorithm)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when ENABLE_findRedCan is activated (default: OFF). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/findRedCan
 *
 * <b>Modify</b>
 *
 * This file can be edited at src/modules/findRedCan/main.cpp
 *
 */

#include "FindRedCan.hpp"

int main(int argc, char** argv) {

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("findRedCan/conf");
    rf.setDefaultConfigFile("findRedCan.ini");
    rf.configure("ASIBOT_ROOT", argc, argv);

    FindRedCan mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"findRedCan --help\" for options.\n");
    printf("findRedCan checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\nfindRedCan found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

