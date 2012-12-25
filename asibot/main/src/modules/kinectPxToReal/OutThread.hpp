// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __OUT_THREAD_HPP__
#define __OUT_THREAD_HPP__

#include <yarp/os/all.h>
#include <yarp/sig/Vector.h>
#include <yarp/math/Math.h>

#include "SharedArea.hpp"

#define THREAD_RATE_INIT 20  // In ms, unmeaningful as ALWAYS gets overwritten by RF

using namespace yarp::os;
using namespace yarp::sig;

class OutThread : public RateThread {
private:
    BufferedPort<Vector> *pfcPort;  // to output the corrected output
    SharedArea *pMem;

public:
    OutThread() : RateThread(THREAD_RATE_INIT) {}  // In ms

    void setSharedArea(SharedArea* _pMem);
    void setOutputPort(BufferedPort<Vector> *_pfcPort);
    void init(ResourceFinder &rf);
    void run();  // The periodical function
};

#endif  // __OUT_THREAD_HPP__

