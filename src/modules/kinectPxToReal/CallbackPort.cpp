// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "OutPort.hpp"

/************************************************************************/
void OutPort::setSharedArea(SharedArea* _pMem) {
    pMem = _pMem;
}

/************************************************************************/
void OutPort::init(ResourceFinder &rf) {

/*    while (pMem->getQ().size() == 0) {
        Time::delay(0.5);
        printf("Waiting for q...\n");
    }

    while (pMem->getF().size() == 0) {
        Time::delay(0.5);
        printf("Waiting for f...\n");
    }

    int period = rf.check("rate",20,"ms ratethread").asInt();
    this->setRate(period);
    this->start();*/
}

/************************************************************************/
void OutPort::run() {
    /*Vector predictFerr = pMachine.predict(pMem->getQ());
    printf("predictFerr: %s\n",predictFerr.toString().c_str());
    Vector currentF = pMem->getF();*/
//    Vector& correctF = pfcPort->prepare();
//    correctF = currentF - predictFerr;
//    pfcPort->write();
}

/************************************************************************/
