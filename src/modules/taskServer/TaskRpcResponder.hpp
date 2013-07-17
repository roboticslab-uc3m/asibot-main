// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_RPC_RESPONDER__
#define __TASK_RPC_RESPONDER__

#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Time.h>
#include <yarp/os/Vocab.h>

#include "TaskGrabCanSim.hpp"

//#define MAX_NUM_MOTORS 100

#define VOCAB_HELP VOCAB4('h','e','l','p')
#define VOCAB_FAILED VOCAB4('f','a','i','l')
#define VOCAB_OK VOCAB2('o','k')
#define VOCAB_RUN VOCAB3('r','u','n')

using namespace yarp::os;

/**
 * @ingroup TaskRpcResponder
 *
 * TaskRpcResponder class implements an xRpcPort responder (callback on RPC).
 */
class TaskRpcResponder : public PortReader {
protected:
    /**
    * Implement the actual responder (callback on RPC).
    */
    virtual bool read(ConnectionReader& connection);

    int *tsStatus;

    TaskGrabCanSim taskGrabCanSim;

public:

    /**
    * Register.
    */
    void setTsStatus(int* _tsStatus);
};

#endif  // __TASK_RPC_RESPONDER__

