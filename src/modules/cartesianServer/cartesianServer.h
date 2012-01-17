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

using namespace yarp::os;
using namespace yarp::dev;

class xPort : public BufferedPort<Bottle> {
protected:
//    sharedArea *pMem;
    virtual void onRead(Bottle& v) {
//        printf("[Debug] Data arrived on qSetPort\n");
//        pMem->setQ(v);
    }
public:
//    void setSharedArea(sharedArea* _pMem) {
//        pMem = _pMem;
//    }
};

//YARP_DECLARE_PLUGINS(rlPlugins);

class cartesianServer : public RFModule {
protected:
    yarp::dev::PolyDriver cartesianDevice;

    yarp::dev::ICartesianControl *icart;

//	qSetPort qPort;  // to connect to robot arm, encoders
//	mSetPort mPort;  // to set its mode from outside
//    sharedArea mem;

//    WBCthread wThread;

//    double getPeriod();
    bool updateModule();
//    bool interruptModule();
//    int period;

public:
    cartesianServer();
    bool configure(ResourceFinder &rf);
};

#endif

