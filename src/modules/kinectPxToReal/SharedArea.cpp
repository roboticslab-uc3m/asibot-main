// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SharedArea.hpp"

/************************************************************************/
void SharedArea::setDepth(const ImageOf<PixelFloat>& _depth);
    depthMutex.wait();
    depth = _depth;
    depthMutex.post();
}

/************************************************************************/
void SharedArea::setPixels(const Bottle& _pixels) {
    pixelsMutex.wait();
    pixels = _pixels;
    pixelsMutex.post();
}

/************************************************************************/
ImageOf<PixelFloat> SharedArea::getDepth() {
    depthMutex.wait();
    ImageOf<PixelFloat> _depth=depth;
    depthMutex.post();
    return _depth;
}

/************************************************************************/
Vector SharedArea::getPixels() {
    pixelsMutex.wait();
    Bottle _pixels=pixels;
    pixelsMutex.post();
    return _pixels;
}

/************************************************************************/

