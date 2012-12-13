// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __EXTRA_CALLBACK_PORT__
#define __EXTRA_CALLBACK_PORT__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Semaphore.h>

using namespace yarp::os;

/**
 * @ingroup ExtraCallbackPort
 *
 * ExtraCallbackPort class implements a port with x callbacks.
 */
class ExtraCallbackPort : public BufferedPort<Bottle> {
protected:
    /**
    * Implement the actual callback.
    */
    void onRead(Bottle& b);

    double dEncRaw[3];

    Semaphore encRawMutex;  // SharedArea

public:

    int dof;

    ExtraCallbackPort() {}

    void setEncExposed(const int Index, const double Position) {
        encRawMutex.wait();
        dEncRaw[Index] = Position;
        encRawMutex.post();
    }

    double getEncRaw(const int Index) {
        encRawMutex.wait();
        double tmp = dEncRaw[Index];
        encRawMutex.post();
        return tmp;
    }

};

#endif

