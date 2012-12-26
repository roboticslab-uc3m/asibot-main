// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CALLBACK_PORT_HPP__
#define __CALLBACK_PORT_HPP__

#include <yarp/os/all.h>
//#include <yarp/sig/Vector.h>
//#include <yarp/math/Math.h>

#include "SharedArea.hpp"

using namespace yarp::os;
using namespace yarp::sig;

class CallbackPort : public BufferedPort<Bottle> {
    private:
        SharedArea* pMem;
        Port* outPort;
        virtual void onRead(Bottle& b);

    public:
        void setSharedArea(SharedArea* _pMem);
        void setOutPort(Port* _outPort);
};

#endif  // __CALLBACK_PORT_HPP__

