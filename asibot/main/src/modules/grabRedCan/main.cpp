// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup grabRedCan
 *
 * \image html grabRedCan.png
 *  <center> <a href="http://youtu.be/ePW25V5i8FE" target="_blank"> Watch video on Youtube</a> </center>
 * 
 * The <b>grabRedCan</b> module reads the <b>geom.ini</b> configuration file found in $ASIBOT_DIR/app/conf and the streaming output of <a href="group__findRedCan.html">findRedCan</a> through a <a href="http://eris.liralab.it/yarpdoc/classyarp_1_1os_1_1Port.html">yarp::os::Port</a> input called <b>/redCan:i</b>. It connects to a <a href="group__test__kdl.html">test_kdl</a> that exposes a YARP remote_controlboard interface and an RPC server for cartesian movements, and makes the robot grab a red can situated at the kitchen environment table using the previously described geometrical information.
 * Figure 1 depicts the port connections made at runtime.
 *
 *  \image html demoSetup.png
 *  <center> Fig. 1 - Port connections made at runtime </center>
 *
 * <b> Legal </b>
 *
 * Copyright: 2011 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
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
 * $ASIBOT_DIR/bin/findRedCan
 *
 * $ASIBOT_DIR/bin/test_kdl --env $RCGv04_DIR/share/models/asibot_redCan.env.xml
 *
 * $ASIBOT_DIR/bin/grabRedCan
 *
 * <b>Modify</b>
 *
 * This file can be edited at 
 * src/modules/findRedCan/main.cpp
 *
 */

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <yarp/os/RpcClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <yarp/dev/PolyDriver.h>

#include <yarp/dev/ControlBoardInterfaces.h>

#include <yarp/dev/CartesianControl.h>
#include <yarp/sig/Vector.h>

#define PI 3.14159265


using namespace yarp::os;
using namespace yarp::dev;

class cartController {
  public:
    RpcClient output;
    Bottle bottle_x_o;
    Bottle bottle_x_i;
    bool init() {
        output.open("/grabCan");
        if(!output.addOutput("/ravebot/x/rpc:i")) {
            printf("[error] I can't find the /ravebot/x/rpc:i port. Please try running $ASIBOT_DIR/bin/test_kdl --env $RCGv04_DIR/share/models/asibot_redCan.env.xml\n");
            return false;
        }
        return true;
    }

    bool moveSome(int type, const char* axis, double value) {
        bottle_x_o.clear();
        bottle_x_o.addInt(type);
        bottle_x_o.addString(axis);
        bottle_x_o.addDouble(value);
        bottle_x_o.addString("roll");
        bottle_x_o.addDouble(0.0);
        bottle_x_o.addString("pitch");
        bottle_x_o.addDouble(0.0);
        bottle_x_o.addString("yaw");
        bottle_x_o.addDouble(0.0);
        int ret = output.write(bottle_x_o,bottle_x_i);
        printf("Got response: %s\n", bottle_x_i.toString().c_str());
        return ret;
    }

    bool setDuration(double value) {
        bottle_x_o.clear();
        bottle_x_o.addInt(4);
        bottle_x_o.addDouble(value);
        int ret = output.write(bottle_x_o,bottle_x_i);
        printf("Got response: %s\n", bottle_x_i.toString().c_str());
        return ret;
    }

    bool waitMotionDone(double timeout) {
        if (timeout == 0) timeout = 10000;
        printf("[waitMotionDone] Begin with timeout = %f seconds.\n",timeout);
        double first = Time::now();
        bool motionDone = false;
        bool timedOut = false;
        bool ret = true;
        while((!motionDone)&&(!timedOut)){
            bottle_x_o.clear();
            bottle_x_o.addInt(0);
            ret = output.write(bottle_x_o,bottle_x_i);
            if((bottle_x_i.get(0).getCode()==BOTTLE_TAG_INT)&&(bottle_x_i.get(0).asInt()<=0))
                motionDone=true;
            if((Time::now()-first) > timeout) {
                timedOut=true;
            }
            if(!timedOut) Time::delay(0.1);  // [s]
        }
        if(!timedOut) printf("[waitMotionDone] Done with elapsed time = %f seconds.\n", (Time::now()-first));
        else printf("[waitMotionDone] Timed out with elapsed time = %f seconds.\n", (Time::now()-first));
        return ret;
    }

    bool getStat(double double_x_i[6]) {
        bottle_x_o.clear();
        bottle_x_o.addInt(0);
        bool ret = output.write(bottle_x_o,bottle_x_i);
        const std::string axis_name[6]={"x","y","z","roll","pitch","yaw"};
        for(int axis=0;axis<6;axis++)
            double_x_i[axis]=bottle_x_i.find(axis_name[axis].c_str()).asDouble();
        return ret;
    }
};


int main(int argc, char *argv[]) {

    Property options;
    options.fromCommand(argc, argv);
    if(options.check("help")) {
        printf("\n");
        printf("Option: --help  -------> This help\n");
        printf("\n");
        printf("Dependencies: yarp server\n");
        printf("Dependencies: $ASIBOT_DIR/bin/findRedCan\n");
        printf("Dependencies: $ASIBOT_DIR/bin/test_kdl --env $RCGv04_DIR/share/models/asibot_redCan.env.xml\n");
        printf("\n");
        exit(1);
    }

    ConstString geomConfigFile(getenv("ASIBOT_DIR"));
    geomConfigFile += "/app/conf/geom.ini";
    Property geometry;
    if(!geometry.fromConfigFile(geomConfigFile)) {
        printf("[error] Need geom.ini\n");
        return 1;
    }
    double robotx = geometry.find("robotx").asDouble();
    double roboty = geometry.find("roboty").asDouble();
    double robotz = geometry.find("robotz").asDouble();
    double latarz = geometry.find("latarz").asDouble();
    double latarx;
    double latary;

    printf("robotx: %f cm (from geom.ini)\n", robotx);
    printf("roboty: %f cm (from geom.ini)\n", roboty);
    printf("robotz: %f cm (from geom.ini)\n", robotz);

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("[error] Please try running yarp server\n");
        return(-1);
    }

    Port input;
    input.open("/redCan:i");
    if(!yarp.connect("/redCan:o","/redCan:i")) {
        printf("[error] I can't find the /redCan:o port. Please try running $ASIBOT_DIR/bin/findRedCan\n");
        input.interrupt();
        input.close();
        Network::fini();
        return 1;
    }
    printf("Waiting to receive from /redCan:o...\n");
    Bottle bot;
    input.read(bot);
//    printf("Got message: %s\n", bot.toString().c_str());
    input.interrupt();
    input.close();
    if(bot.get(1).toString()=="roja") {
        latarx = bot.get(2).asDouble();
        latary = bot.get(3).asDouble();
        printf("latarx: %f cm (from findRedCan)\n", latarx);
        printf("latary: %f cm (from findRedCan)\n", latary);
        printf("latarz: %f cm (from geom.ini)\n", latarz);
    } else {
        printf("No red can detected.\n");
        return -1;
    }
    printf("This would be like having red can at (%f %f %f) in RaveBot.\n",
        2.2-(latary/100.0),0.2+(latarx/100.0),0.045+(latarz/100.0)); // 0.045 m is the Rave offset
        // needed because the can in RaveBot env is defined in space by its COG.

    printf("Performing secuence like having robot at (%f %f %f) in RaveBot.\n\n",
        2.2-(roboty/100.0),0.2+(robotx/100.0),0.0+(robotz/100.0));  // 0.0 m is the Rave offset
        // needed because the robot in RaveBot env is defined in space by its COG.

    double catetoOpuesto = robotx - latarx;
    double catetoContiguo = roboty - latary;
    double giro = -atan(catetoOpuesto/catetoContiguo) * 180 / PI;
    double RCdistFromBase = sqrt(pow(catetoOpuesto,2)+pow(catetoContiguo,2))/100.0;
    double RCheightFromBase = (latarz-robotz)/100.0;
    printf ("The arc tangent is %f degrees\n", giro);
    printf ("Distance from robot base to red can is %f m\n", RCdistFromBase);
    printf ("Height from robot base to red can is %f m\n", RCheightFromBase);
/*    double catetoOpuesto = (0.2+(latarx/100.0)) - (0.2+(robotx/100.0));
    double catetoContiguo = (2.2-(roboty/100.0)) - 2.2-(latary/100.0);
    double giro = (atan(catetoOpuesto/catetoContiguo) * 180 / PI) - 90.0;
    printf ("The arc tangent is %lf degrees\n", giro);*/


///////// Next 3 lines create "ravebot" device which may be used remote and locally //////////////
//    options.put("device","controlboard");
//    options.put("subdevice","ravebot");
//    options.put("name","/ravebot");
//    // options.put("env","blablablablabla");  // would be nice?
/////////// Next 3 lines connect to a remote "ravebot" //////////////////////////////
    // Here I am assuming there is a ./raveopen
    options.put("device","remote_controlboard");  
    options.put("remote","/ravebot");
    options.put("local","/local");

    PolyDriver dd(options);
    if(!dd.isValid()) {
      printf("[error] ravebot device not available.\n");
      printf("[error] Please try running $ASIBOT_DIR/bin/test_kdl --env $RCGv04_DIR/share/models/asibot_redCan.env.xml\n");
	  dd.close();
      Network::fini();
      return 1;
    }

    IPositionControl *pos;
    dd.view(pos);

    int jnts = 0;
    pos->getAxes(&jnts);
    printf("axes: %d\n", jnts);

    pos->positionMove(0, giro-0.275); // offset (0.5 se pasa a der), 0.25 izq, 0.3 a izq
    bool motionDone;
    for (unsigned int i=0;i<jnts;i++) {
        motionDone = false;
        while(!motionDone){
            pos->checkMotionDone(i,&motionDone);
            Time::delay(0.1);  // [s]
        }
        printf("Joint q%d reached target.\n",i+1);
    }
    pos->positionMove(1, -10);
    pos->positionMove(2, 20);
    pos->positionMove(3, 80);

    for (unsigned int i=0;i<jnts;i++) {
        motionDone = false;
        while(!motionDone){
            pos->checkMotionDone(i,&motionDone);
            Time::delay(0.1);  // [s]
        }
        printf("Joint q%d reached target.\n",i+1);
    }
    dd.close();
    printf("Done init pose! Starting with aproximation scheme...\n");


    Port toolPort;
    toolPort.open("/redcan/tool:o");
    if(!yarp.connect("/redcan/tool:o","/ravebot/tool:i")) {
        printf("tool not available on ravebot device.\n");
        printf("Note: Please try running $ASIBOT_DIR/bin/test_kdl --env $RCGv04_DIR/share/models/asibot_redCan.env.xml\n");
        toolPort.interrupt();
        toolPort.close();
        Network::fini();
        return 1;
    }
    Bottle bt;
    bt.clear();
    bt.addDouble(-25.0); // open tool, was good at -10.0 and -20.0
    toolPort.write(bt);
//    Time::delay(1); // we have no feedback on tool

    cartController miniController;
    printf("Start moving in cartesian, will exit if comm error occurs.\n");
    if(!miniController.init()) return -1;
    if(!miniController.setDuration(30.0)) return -1; // make it go slower
    if(!miniController.moveSome(2,"z",-0.45)) return -1;
    if(!miniController.waitMotionDone(45)) return -1;
    if(!miniController.moveSome(3,"z",0.3)) return -1;
    if(!miniController.waitMotionDone(45)) return -1;
    double axes[6];
    miniController.getStat(axes);
    printf("End effector at KDL (%f %f %f) m\n",axes[0],axes[1],axes[2]);
    double EEdistFromBase = sqrt(pow(axes[0],2)+pow(axes[1],2));
    double EEheightFromBase = axes[2]+0.063; // +0.16 because KDL lacks first link length
    // 0.08 a tiny bit too low. 0.075 perfect as is. 0.05 too high, 0.04 too high, 0.03 way too high
    // 0.078 too low in stable. 0.07 also too low in stable. 0.065 exactly touches. 0.064 too high?
    // 0.065 too low. 0.0649 too low. 0.064 too low. 0.063 too low. 0.062 tad bit high, 0.0621 hits.
    //
    // 0.062 too high. 0.0635 too high. 0.0655 too high, as 0.068, 0.09 peta mesa
    //
    // 0.063 va arrastrando igual q 0.062, 0.061 lo roza por nada, 0.06000 perfecto en estable (0.0575)
    printf("End effector at [dist: %f m] [height %f m] from robot base\n",EEdistFromBase,EEheightFromBase);
    printf("Red can at [dist: %f m] [height %f m] from robot base\n",RCdistFromBase,RCheightFromBase);
    printf("Decending [height %f m] to equal heights...\n",EEheightFromBase-RCheightFromBase);
    if(!miniController.moveSome(2,"z",-(EEheightFromBase-RCheightFromBase))) return -1;
    if(!miniController.waitMotionDone(60)) return -1;
    if(!miniController.setDuration(30.0)) return -1; // make it go slower
    if(!miniController.moveSome(3,"z",0.1055)) return -1; //30.0+0.17 too much; .12 was OK,.8 not enoguh
    // 0.175 wasnt too bad either. 0.15 not enough. 0.01 not en. 0.105 too much.
    //
    // 0.1025 needs a bit
    if(!miniController.waitMotionDone(60)) return -1;
    Time::delay(5); // we have no feedback on tool
    bt.clear();
//    bt.addDouble(-15.0); // close tool a bit, -7 wasnt enogh
    bt.addDouble(-8.8);  // -15 too open, as -12. -8 too closed, -10 was OK, as -9.5
    // -9.4 too open; -9.2 not bad, as -9, as -8.9
    toolPort.write(bt);
//    Time::delay(4); // we have no feedback on tool
//    if(!miniController.setDuration(10.0)) return -1; // make it go slower
//    if(!miniController.moveSome(3,"z",0.1)) return -1; //30.0+0.17 too much
    Time::delay(5); // we have no feedback on tool
    if(!miniController.setDuration(60.0)) return -1; // make it go slower
    miniController.bottle_x_o.clear();
    miniController.bottle_x_o.addInt(3);
    miniController.bottle_x_o.addString("pitch");
    miniController.bottle_x_o.addDouble(-30.0);
    miniController.bottle_x_o.addString("roll");
    miniController.bottle_x_o.addDouble(0.0);
    miniController.bottle_x_o.addString("yaw");
    miniController.bottle_x_o.addDouble(0.0);
//    miniController.bottle_x_o.addString("z");
//    miniController.bottle_x_o.addDouble(0.01);
    miniController.output.write(miniController.bottle_x_o,miniController.bottle_x_i);
    printf("Got response: %s\n", miniController.bottle_x_i.toString().c_str());
    if(!miniController.waitMotionDone(120)) return -1;
    Time::delay(1);// we have no feedback on tool
    printf("yeah!\n");       
    if(!miniController.setDuration(70.0)) return -1; // make it go slower
    if(!miniController.moveSome(3,"x",-0.1)) return -1;
    if(!miniController.waitMotionDone(90)) return -1;
    if(!miniController.setDuration(0.0)) return -1; // return to normal speed
    printf("Done! Leaving...\n");
    return 0;
}


