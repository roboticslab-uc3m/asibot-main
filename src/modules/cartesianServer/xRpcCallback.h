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
using namespace yarp::sig;

/**
 * @ingroup xRpcCallback
 *
 * xRpcCallback class implements an xRpcPort responder (callback on RPC).
 */
class xRpcCallback : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

    yarp::dev::ICartesianControl *icart;
    yarp::dev::IPositionControl *ipos;
public:

    /**
    * Register a cartesian interface for the PortReader.
    */
    void setCartesianInterface(yarp::dev::ICartesianControl* _icart);

    /**
    * Register a position interface for the PortReader.
    */
    void setPositionInterface(yarp::dev::IPositionControl* _ipos);
};

#endif

