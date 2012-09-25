// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __X_CALLBACK_PORT__
#define __X_CALLBACK_PORT__

#define _USE_MATH_DEFINES  // this is only needed in Windows

#include <math.h>

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#define VPOINT_DIST 0.05  // [m]

#define VOCAB_MY_STOP VOCAB4('s','t','o','p')

#define VOCAB_STAT VOCAB4('s','t','a','t')
#define VOCAB_MOVL VOCAB4('m','o','v','l')
#define VOCAB_MOVJ VOCAB4('m','o','v','j')
#define VOCAB_INV VOCAB3('i','n','v')

#define VOCAB_FWD VOCAB3('f','w','d')
#define VOCAB_BKWD VOCAB4('b','k','w','d')
#define VOCAB_ROT VOCAB3('r','o','t')
#define VOCAB_VMOS VOCAB4('v','m','o','s')

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

/**
 * @ingroup xCallbackPort
 *
 * xCallbackPort class implements a port with x callbacks.
 */
class xCallbackPort : public BufferedPort<Bottle> {
protected:
    /**
    * Implement the actual callback.
    */
    void onRead(Bottle& b);

    yarp::dev::ICartesianControl *icart;
    yarp::dev::IPositionControl *ipos;
public:

    xCallbackPort() {}

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

