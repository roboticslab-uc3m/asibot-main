// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __OUT_PORT_HPP__
#define __OUT_PORT_HPP__

#include <yarp/os/all.h>
//#include <yarp/sig/Vector.h>
//#include <yarp/math/Math.h>

#include "SharedArea.hpp"

using namespace yarp::os;
using namespace yarp::sig;

class OutPort : public BufferedPort<Bottle> {
    private:
        SharedArea *pMem;

    public:
        void setSharedArea(SharedArea* _pMem);
        // void init(ResourceFinder &rf);
        // void run();  // The periodical function
};

#endif  // __OUT_PORT_HPP__

