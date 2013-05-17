// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * @ingroup asibot_modules
 * \defgroup colorSegmentor colorSegmentor
 *
 * @brief The colorSegmentor module provides basic 2D image feature extraction. It uses the \ref VisionSegmentorLib library.
 *
 * The colorSegmentor module provides basic 2D image feature extraction. It uses the \ref VisionSegmentorLib library.
 * The current feature order is the following:
 \verbatim
 ((0 reserved for timestamps))
----------------------------------- options: -------------------------------------
 massCenterlocX
 massCenterlocY
 aspectRatio
 area
 rectangularity
 axisFirst
 axisSecond
 solidity
 arc
 radius
 hue_peak ((red: 0/180, green: 60; blue: 120))
 value_peak
 hue_mean
 hue_stddev
 saturation_peak
 saturation_mean
 saturation_stddev
 value_mean
 value_stddev
 locX
 locY
 hue_mode
 hue_mode \endverbatim
 *
 * @section colorSegmentor_legal Legal
 *
 * Copyright: 2013 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a>,
 * <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 3.0 or later
 *
 * @section colorSegmentor_install Installation
 *
 * The module is compiled when ENABLE_colorSegmentor is activated (default: ON). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <hr>
 *
 * This file can be edited at $ASIBOT_ROOT/main/src/modules/colorSegmentor/main.cpp
 *
 */

#include "VisionSegmentor.hpp"

int main(int argc, char** argv) {

    ResourceFinder rf;
    //rf.setVerbose(true);
    //rf.setDefaultContext("colorSegmentor/conf");
    //rf.setDefaultConfigFile("colorSegmentor.ini");
    //rf.configure("ASIBOT_ROOT", argc, argv);

    VisionSegmentor mod;
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

