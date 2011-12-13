// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup test_kdl
 *
 * Creates an instance of RaveBot, and opens a <a href="http://eris.liralab.it/yarpdoc/classyarp_1_1os_1_1RpcServer.html">yarp::os::RpcServer</a> for cartesian movements.
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
 * installation steps refer to <a href="pages.html">you own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/test_kdl
 *
 * <b>Modify</b>
 * 
 * This file can be edited at 
 * src/libraries/ravebot/test_ravebot.cpp
 *
 */

#include "test_kdl.h"
#include <stdio.h>
//#include <iostream>

YARP_DECLARE_PLUGINS(rlPlugins);

int main(int argc, char *argv[]) {
    Property config;
    config.fromCommand(argc, argv);
    if (config.check("help")) {
        printf("\n");
        printf("Option: --help  -----> This help\n");
        printf("Option: --xpcport [/xrpcport] -----> Defaults to /ravebot/x/rpc:i\n");
        printf("Option: --env [abs_or_rel_path_to_env] -----> Defaults to asibot.env.xml\n");
        printf("\n");
        exit(1);
    }
    // Object construction
    Network yarp;
    if (!Network::checkNetwork()) {
        printf("Please try running yarp server\n");
        return(-1);
    }

    YARP_REGISTER_PLUGINS(rlPlugins);
    RpcServer port_x;
    ControlLoop theControlLoop(config);
    // Object initialization
    Value xportvalue = config.check("xport","/ravebot/x/rpc:i","checking if given an alternate x port name");
    port_x.open(xportvalue.asString());
    theControlLoop.start();
    // Welcome message
    printf("[cmc_kdl] Welcome to cmc_kdl module! CLOUD initialized!\n");
    ////////////////////////////////////// MAIN LOOP ///////////////////////////////////
    while (true) {
        Bottle bottle_x_i;
        Bottle bottle_x_o;
        port_x.read(bottle_x_i, true);
        printf("Recieved %s.\n", bottle_x_i.toString().c_str());
        int choice = bottle_x_i.get(0).asInt();
        if (bottle_x_i.get(0).getCode() != BOTTLE_TAG_INT) choice = -2;
        if (choice==-1) {  ///////////////////////////////// x -1 /////////////////////////////////
            bottle_x_o = theControlLoop.emergencyStop();
            port_x.reply(bottle_x_o);
        } else if (choice==0) { ///////////////////////////////// x 0 /////////////////////////////////
            bottle_x_o = theControlLoop.controlStat();
            port_x.reply(bottle_x_o);
        } else if ((choice==1)||(choice==2)||(choice==3)) { /////////////// x 1,2,3 ///////////////////////
            double double_x_i[6]={0,0,0,0,0,0};
            bool bool_x_i[6]={false,false,false,false,false,false};
            double velocity = 100.0; // 100% if no 'vgen' tag.
            const std::string axis_name[6]={"x","y","z","roll","pitch","yaw"};
            for (int axis=0; axis<6; axis++)
                if (!bottle_x_i.find(axis_name[axis].c_str()).isNull()) {
                    bool_x_i[axis]=true;
                    double_x_i[axis]=bottle_x_i.find(axis_name[axis].c_str()).asDouble();
                }
            if (!bottle_x_i.find("vgen").isNull())
                velocity = bottle_x_i.find("vgen").asDouble();
            if (choice==1) {
                if (theControlLoop.controlMovAbs(bool_x_i,double_x_i, velocity))
                    bottle_x_o.addVocab(VOCAB_OK);
                else bottle_x_o.addVocab(VOCAB_FAILED);
                port_x.reply(bottle_x_o);
            } else if (choice==2) {
                if (theControlLoop.controlMovRelBase(bool_x_i,double_x_i, velocity))
                    bottle_x_o.addVocab(VOCAB_OK);
                else bottle_x_o.addVocab(VOCAB_FAILED);
                port_x.reply(bottle_x_o);
            } else if (choice==3) {
                if (theControlLoop.controlMovRelEE(bool_x_i,double_x_i, velocity))
                    bottle_x_o.addVocab(VOCAB_OK);
                else bottle_x_o.addVocab(VOCAB_FAILED);
                port_x.reply(bottle_x_o);
            }
        } else if (choice==4) {
            if (bottle_x_i.get(1).getCode() == BOTTLE_TAG_DOUBLE) {
                theControlLoop.setTrajTime(bottle_x_i.get(1).asDouble());
                bottle_x_o.addVocab(VOCAB_OK);
            } else bottle_x_o.addVocab(VOCAB_FAILED);
            port_x.reply(bottle_x_o);
        } else {
            bottle_x_o.addVocab(VOCAB_FAILED);
            port_x.reply(bottle_x_o);
        }
    }
    return 0;
}
