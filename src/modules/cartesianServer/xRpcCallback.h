// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef WBC_DEMO
#define WBC_DEMO

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

using namespace yarp::os;
using namespace yarp::dev;

// xRpcCallback class will help us create a callback&rpc port
class xRpcCallback : public PortReader {
protected:

    virtual bool read(ConnectionReader& connection);

    yarp::dev::ICartesianControl *icart;
public:

    void setCartesianInterface(yarp::dev::ICartesianControl* _icart);
};

#endif

