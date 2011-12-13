// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * \defgroup test_printbot
 *
 * Creates an instance of PrintBot.
 *
 * <b> Legal </b>
 *
 * Copyright: 2010 (C) Universidad Carlos III de Madrid; 2006 (C) RobotCub Consortium
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
 * $ASIBOT_DIR/bin/test_printbot
 *
 * <b>Modify</b>

 * This file can be edited at 
 * src/libraries/printbot/PrintBot.h
 *
 */

#include <stdio.h>
#include <yarp/os/Log.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/Terminator.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ServiceInterfaces.h>
#include <yarp/dev/Drivers.h>

#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_unistd.h> 
#include <ace/OS_NS_signal.h>
#include <ace/Vector_T.h>

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlPlugins);

static ConstString terminatorKey = "";
static bool terminated = false;
static void handler (int) {
    static double handleTime = -100;
    static int ct = 0;
    double now = Time::now();
    if (now-handleTime<1) {
        return;
    }
    handleTime = now;
    ct++;
    if (ct>3) {
        if (yarp_show_info()) {
            printf("Aborting...\n");
        }
        ACE_OS::exit(1);
    }
    if (terminatorKey!="") {
        if (yarp_show_info()) {
            printf("[try %d of 3] Trying to shut down %s\n", 
                   ct,
                   terminatorKey.c_str());
        }
        terminated = true;
        Terminator::terminateByName(terminatorKey.c_str());
    } else {
        if (yarp_show_info()) {
            printf("Aborting...\n");
        }
        ACE_OS::exit(1);
    }
}

int main(int argc, char *argv[]) {

    ACE_OS::signal(SIGINT, (ACE_SignalHandler) handler);
    ACE_OS::signal(SIGTERM, (ACE_SignalHandler) handler);

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    YARP_REGISTER_PLUGINS(rlPlugins);

    // check our device can be wrapped in the controlboard network wrapper
    // and accessed remotely
    printf("================================\n");
    printf("check our device can be accessed\n");

    Property options;

    options.fromCommand(argc, argv);  // works!!

    options.put("device","controlboard");
    options.put("subdevice","printbot");
    options.put("name","/printbot");

    PolyDriver dd(options);
    
    if (!dd.isValid()) {
        printf("[error] Server instantiation not worked.\n\n");
        printf("Be sure CMake \"ENABLE_rlYarpmods_printbot\" variable is set \"ON\"\n");
        printf("\"SKIP_printbot is set\" --> should be --> \"ENABLE_printbot is set\"\n\n");
        dd.close();
        return -1;
    }

    Terminee *terminee = 0;  // Doesn't do much without ACE handler...
    if (dd.isValid()) {
        Value *v;
        ConstString s("/yarpdev/quit");
        if (options.check("device", v)) {
            if (v->isString()) {
                s = "";
                s += "/";
                s += v->toString();
                s += "/quit";
            }
        }
        if (options.check("name", v)) {
            s = "";
            s += v->toString();
            s += "/quit";
        }
        terminee = new Terminee(s.c_str());
        terminatorKey = s.c_str();
        if (terminee == 0) {
            printf("Can't allocate terminator port\n");
            terminatorKey = "";
            dd.close();
            return 1;
        }
        if (!terminee->isOk()) {
            printf("Failed to create terminator port\n");
            terminatorKey = "";
            delete terminee;
            terminee = NULL;
            dd.close();
            return 1;
        }
    }

    double dnow = 3;
    double startTime = Time::now()-dnow;
    IService *service = NULL;
    dd.view(service);
    if (service!=NULL) {
        bool backgrounded = service->startService();
        if (backgrounded) {
            // we don't need to poll this, so forget about the
            // service interface
            printf("test_printbot: service backgrounded\n");
            service = NULL;
        }
    }
    while (dd.isValid() && !(terminated||terminee->mustQuit())) {
        if (service!=NULL) {
            double now = Time::now();
            if (now-startTime>dnow) {
                printf("test_printbot: device active...\n");
                fflush(stdout);
                startTime += dnow;
            }
            // we requested single threading, so need to
            // give the device its chance
            service->updateService();
        } else {
            // we don't need to do anything
            printf("test_printbot: device active in background...\n");
            fflush(stdout);
            Time::delay(dnow);
        }
    }

    delete terminee;
    dd.close();
    return 0;
}

