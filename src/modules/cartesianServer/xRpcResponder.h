// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef WBC_DEMO
#define WBC_DEMO

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#define VOCAB_MY_STOP VOCAB4('s','t','o','p')
#define VOCAB_STAT VOCAB4('s','t','a','t')
#define VOCAB_MOVL VOCAB4('m','o','v','l')
#define VOCAB_MOVJ VOCAB4('m','o','v','j')
#define VOCAB_INV VOCAB3('i','n','v')

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

/**
 * @ingroup xRpcResponder
 *
 * xRpcResponder class implements an xRpcPort responder (callback on RPC).
 */
class xRpcResponder : public PortReader {
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

