// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_SERVER__
#define __CARTESIAN_SERVER__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>

#define DEFAULT_CONTROLLER "cartesianbot"

using namespace yarp::os;
using namespace yarp::dev;

class xCommPort : public BufferedPort<Bottle> {
protected:
    yarp::dev::ICartesianControl *icart;
    virtual void onRead(Bottle& b) {
        printf("[Debug] Data arrived on xCommPort\n");
        printf("Recieved %s.\n", b.toString().c_str());
        int choice = b.get(0).asInt();
        if (b.get(0).getCode() != BOTTLE_TAG_INT) choice = -2;
        if (choice==-1) {  ///////////////////////////////// x -1 /////////////////////////////////
            printf("T1\n");
            icart->stopControl();
//            bottle_x_o = theControlLoop.emergencyStop();
//            port_x.reply(bottle_x_o);
        } else if (choice==0) { ///////////////////////////////// x 0 /////////////////////////////////
//            bottle_x_o = theControlLoop.controlStat();
//            port_x.reply(bottle_x_o);
        }
    }
public:
    void setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
        icart = _icart;
    }

};

class cartesianServer : public RFModule {
protected:
    yarp::dev::PolyDriver cartesianDevice;

    yarp::dev::ICartesianControl *icart;

	xCommPort xPort;

    bool updateModule();
    bool interruptModule();
//    double getPeriod();
//    int period;

public:
    cartesianServer();
    bool configure(ResourceFinder &rf);
};

#endif

