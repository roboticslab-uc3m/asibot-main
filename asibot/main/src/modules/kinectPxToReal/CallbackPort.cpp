// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CallbackPort.hpp"

/************************************************************************/
void CallbackPort::setSharedArea(SharedArea* _pMem) {
    pMem = _pMem;
}

/************************************************************************/
void CallbackPort::setParams(double _fx, double _fy, double _cx, double _cy) {
    fx = _fx;
    fy = _fy;
    cx = _cx;
    cy = _cy;
}

/************************************************************************/
void CallbackPort::setOutPort(Port* _outPort) {
    outPort = _outPort;
}

/************************************************************************/

void CallbackPort::onRead(Bottle& b) {
    printf("[CallbackPort] Got %s\n", b.toString().c_str());
    Bottle outLists;
    for (int i=0; i<b.size(); i++) {
        Bottle* pxCoords = b.get(i).asList();
        int pxX = pxCoords->get(0).asInt();
        int pxY = pxCoords->get(1).asInt();
        ImageOf<PixelFloat> depth = pMem->getDepth();
        double mmZ = depth(pxX,pxY);
        Bottle mmOut;
        mmOut.addInt(pxX);  //TODO: pass to mm
        mmOut.addInt(pxY);  //TODO: pass to mm
        mmOut.addDouble(mmZ);
        outLists.addList() = mmOut;
    }
    outPort->write(outLists);
}

/************************************************************************/

