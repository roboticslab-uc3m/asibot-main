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
    //fprintf(stdout,"[CallbackPort] Got %s\n", b.toString().c_str());
    Bottle outLists;
    for (int i=0; i<b.size(); i++) {
        Bottle* pxCoords = b.get(i).asList();
        double pxX = pxCoords->get(0).asDouble();
        double pxY = pxCoords->get(1).asDouble();
        ImageOf<PixelFloat> depth = pMem->getDepth();
        double mmZ = depth((int)pxX,(int)pxY);  // maybe better do a mean around area?
        Bottle mmOut;
        double mmX = 1000.0 * (pxX - (cx * mmZ/1000.0)) / fx;
        double mmY = 1000.0 * (pxY - (cy * mmZ/1000.0)) / fy;
        mmOut.addDouble(mmX);
        mmOut.addDouble(mmY);
        mmOut.addDouble(mmZ);
        if(mmZ != 0) outLists.addList() = mmOut;
    }
    outPort->write(outLists);
}

/************************************************************************/

