// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SHARED_AREA_HPP__
#define __SHARED_AREA_HPP__

#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>

using namespace yarp::os;
using namespace yarp::sig;

class SharedArea {
private:
    Vector q;
    Vector f;
    Semaphore qMutex;
    Semaphore fMutex;
public:
    void setF(const Vector& v);
    void setQ(const Vector& v);
    Vector getF();
    Vector getQ();
};

#endif  // __SHARED_AREA_HPP__

