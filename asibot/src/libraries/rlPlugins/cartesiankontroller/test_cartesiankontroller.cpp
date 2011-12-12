// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup test_cartesiankontroller
 *
 *
 * <b>[DEPRECIATED]</b>
 *
 * <b>Please use <a href="group__test__kdl.html">test_kdl</a> instead.</b>
 *
 * Creates an instance of CartesianKontroller.
 *
 * Universidad Carlos III de Madrid (C) 2010;
 * RobotCub Consortium (C) 2006
 *
 * Author: Juan G. Victores
 *
 * Contrib: Paul Fitzpatrick (YARP sample code, email responses);
 *          Rosen Diankov (OpenRAVE sample code, email responses)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * This file can be edited at 
 * src/libraries/ravebot/test_ravebot.cpp
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

#include <yarp/dev/CartesianControl.h>
#include <yarp/sig/Vector.h>

#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_unistd.h> 
#include <ace/OS_NS_signal.h>
#include <ace/Vector_T.h>

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlPlugins);

class RpcPort : public BufferedPort<Bottle> {
 public:
  RpcPort(ICartesianControl *_rpcCart);
 private:
  virtual void onRead(Bottle& b);
  ICartesianControl *rpcCart;
};

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

/* ==================================================== */

int main(int argc, char *argv[]) {

    ACE_OS::signal(SIGINT, (ACE_SignalHandler) handler);
    ACE_OS::signal(SIGTERM, (ACE_SignalHandler) handler);

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    YARP_REGISTER_PLUGINS(rlPlugins);

    printf("[test] ===================================================\n");
    printf("[test] creating instance of our cartesiankontroller device\n");

    Property options;
    options.fromCommand(argc, argv);
    options.put("device","cartesiankontroller");
    PolyDriver dd(options);
    
    if (!dd.isValid()) {
        printf("[test] [error] cartesianKontroller instantiation not worked\n\n");
        printf("[test] be sure CMake \"ENABLE_cartesiankontroller\" variable is set \"ON\"\n");
        printf("[test] \"SKIP_cartesiankontroller is set\" --> should be --> \"ENABLE_cartesianKontroller is set\"\n\n");
        printf("[test] you should also make sure your remote_controlboard is running and using its correct name\n\n");
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
            printf("test_rlYarpmods: service backgrounded\n");
            service = NULL;
        }
    }

    // Now open an rpc:i port as we're not going to be doing anything else autonomously from here
    ICartesianControl *cart;
    dd.view(cart);
    yarp::sig::Vector myx;
    yarp::sig::Vector myo;
    yarp::sig::Vector mystate;

    BufferedPort<yarp::sig::Vector>  portState;
    RpcPort portRpc(cart);
    portRpc.useCallback();

    ConstString prefixName="/cartesiankontroller";
    Value v = options.check("robot",Value("/canbot"));
    prefixName=prefixName+v.toString();
    portRpc.open((prefixName+"/rpc:i").c_str());

    ConstString stateName="/cartesiankontroller";
    stateName = stateName + v.toString() + "/state:o";
    portState.open(stateName.c_str());

    /* Main loop */
    while (dd.isValid() && !(terminated||terminee->mustQuit())) {
        if (service!=NULL) {
            double now = Time::now();
            if (now-startTime>dnow) {
                printf("test_rlYarpmods: device active...\n");
                fflush(stdout);
                startTime += dnow;
            }
            // we requested single threading, so need to
            // give the device its chance
            service->updateService();
        } else {
            // we don't need to do anything
            Time::delay(dnow);
            cart->getPose(myx,myo);
            printf(" POS:  %s  |  ORI:  %s\n",myx.toString().c_str(),myo.toString().c_str());
            mystate.clear();
            mystate.push_back(myx(0));
            mystate.push_back(myx(1));
            mystate.push_back(myx(2));
            mystate.push_back(myo(0));
            mystate.push_back(myo(1));
            mystate.push_back(myo(2));
            mystate.push_back(myo(3));
            //printf(" POS+ORI:  %s\n",mystate.toString().c_str());
            portState.prepare() = mystate;
            portState.write();
            // printf("test_rlYarpmods: device active in background...\n");
            fflush(stdout);
        }
    }

    delete terminee;
    dd.close();
    return 0;
}

RpcPort::RpcPort(ICartesianControl *_rpcCart){
   rpcCart = _rpcCart;
};

void RpcPort::onRead(Bottle& b){
  int num_axis = 0;
  for(int axis=0;axis<7;axis++) {
    Value& element = b.get(axis);
    if(element.getCode()==BOTTLE_TAG_DOUBLE) ++num_axis;
  }
  printf("Num of doubles: %d.\n",num_axis);
  if(num_axis==3){
    yarp::sig::Vector xd;
    xd.resize(3);
    xd[0] = b.get(0).asDouble();
    xd[1] = b.get(1).asDouble();
    xd[2] = b.get(2).asDouble();
    rpcCart->goToPosition(xd);
  } else if (num_axis==7) {
    yarp::sig::Vector xd;
    yarp::sig::Vector od;
    xd.resize(3);
    xd[0] = b.get(0).asDouble();
    xd[1] = b.get(1).asDouble();
    xd[2] = b.get(2).asDouble();
    od.resize(4);
    od[0] = b.get(3).asDouble();
    od[1] = b.get(4).asDouble();
    od[2] = b.get(5).asDouble();
    od[3] = b.get(6).asDouble();
    rpcCart->goToPose(xd,od);
  }
};

