// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WORLD_RPC_RESPONDER__
#define __WORLD_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#define VOCAB_HELP VOCAB4('h','e','l','p')
#define VOCAB_MY_STOP VOCAB4('s','t','o','p')
#define VOCAB_STAT VOCAB4('s','t','a','t')
#define VOCAB_MOVL VOCAB4('m','o','v','l')
#define VOCAB_MOVJ VOCAB4('m','o','v','j')
#define VOCAB_INV VOCAB3('i','n','v')
#define VOCAB_FAILED VOCAB4('f','a','i','l')

using namespace yarp::os;

/**
 * @ingroup WorldRpcResponder
 *
 * WorldRpcResponder class implements a worldRpcServer responder (callback on RPC).
 */
class WorldRpcResponder : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

public:

    /**
    * Register a cartesian interface for the PortReader.
    */
    //void setCartesianInterface(yarp::dev::ICartesianControl* _icart);

    /**
    * Register a position interface for the PortReader.
    */
    //void setPositionInterface(yarp::dev::IPositionControl* _ipos);
};

#endif  // __WORLD_RPC_RESPONDER__

