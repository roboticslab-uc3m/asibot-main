// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SharedArea.hpp"

/************************************************************************/
void SharedArea::setDepth(const ImageOf<PixelFloat>& _depth) {
    depthMutex.wait();
    depth = _depth;
    depthMutex.post();
}

/************************************************************************/
ImageOf<PixelFloat> SharedArea::getDepth() {
    depthMutex.wait();
    ImageOf<PixelFloat> _depth=depth;
    depthMutex.post();
    return _depth;
}

/************************************************************************/

