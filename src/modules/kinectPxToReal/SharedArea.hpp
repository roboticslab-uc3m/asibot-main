// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SHARED_AREA_HPP__
#define __SHARED_AREA_HPP__

#include <yarp/os/Semaphore.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;

class SharedArea {
private:
    ImageOf<PixelFloat> depth;
    Semaphore depthMutex;
public:
    void setDepth(const ImageOf<PixelFloat>& _depth);
    ImageOf<PixelFloat> getDepth();
};

#endif  // __SHARED_AREA_HPP__

