// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup asibot_modules
 * \defgroup premultH premultH
 *
 * premultH means Premultiply Homogeneous Transformation Matrix
 *
 * <b> Legal </b>
 *
 * Copyright:  2012 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see repos/asibot-main/doc/LGPL.TXT
 *
 */

#include "PremultH.hpp"

int main(int argc, char *argv[]) {

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("premultH/conf");
    rf.setDefaultConfigFile("premultH.ini");
    rf.configure(argc, argv);

    PremultH mod;
    if(rf.check("help")) {
        return mod.runModule(rf);
    }

    printf("Run \"premultH --help\" for options.\n");
    printf("premultH checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        fprintf(stderr, "[fail]\npremultH found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    return mod.runModule(rf);
}

