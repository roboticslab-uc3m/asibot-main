// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __SHARED_AREA_HPP__
#define __SHARED_AREA_HPP__

#include <yarp/os/Semaphore.h>
#include <yarp/sig/Vector.h>

using namespace yarp::os;
using namespace yarp::sig;

class SharedArea {
private:
    ImageOf<PixelFloat> depth;
    Bottle pixels;
    Semaphore depthMutex;
    Semaphore pixelsMutex;
public:
    void setDepth(const ImageOf<PixelFloat>& _depth);
    void setPixels(const Bottle& _pixels);
    ImageOf<PixelFloat> getDepth();
    Bottle getPixels();
};

#endif  // __SHARED_AREA_HPP__

