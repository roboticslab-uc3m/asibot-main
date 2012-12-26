// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __DEPTH_PORT_HPP__
#define __DEPTH_PORT_HPP__

#include <yarp/os/all.h>
#include <yarp/sig/Vector.h>
#include <yarp/math/Math.h>

#include "SharedArea.hpp"

class DepthPort : public BufferedPort<ImageOf<PixelFloat>> {
    protected:
        sharedArea *pMem;
        virtual void onRead(ImageOf<PixelFloat>& depth) {
            printf("[DepthPort] Data arrived, height: %d, width: %d.\n",depth.height(),depth.width());
            pMem->setDepth(depth);
        }
    public:
        void setSharedArea(sharedArea* _pMem) {
            pMem = _pMem;
        }
};

#endif  // __DEPTH_PORT_HPP__

