// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WiimoteYarp.h"

/************************************************************************/
void WiimoteYarp::setSharedArea(SharedArea* _pSharedArea) {
    pSharedArea = _pSharedArea;
}

/************************************************************************/
bool WiimoteYarp::open() {

//    printf("Run \"wiimoteServer --help\" for options.\n");
    printf("wiimoteServer checking for yarp network... ");
    fflush(stdout);

    if (!yarp.checkNetwork()) {
        printf("[fail]\nwebInterface found no yarp network (try running \"yarpserver &\"), bye!\n");
        return false;
    } else printf("[ok]\n");

    port.open("/wiimoteServer/command:o");

    msPortOut = DEFAULT_MSPORTOUT;
    printf("wiimoteServer using msPortOut \"300\" ms\n",msPortOut);
    this->setRate(msPortOut);
    pSharedArea->setCmd(VOCAB_NULL);
    this->start();

    return true;
}

/************************************************************************/
bool WiimoteYarp::close() {

    port.interrupt();

    this->stop();

    port.close();

    yarp.fini();

    return true;
}

/************************************************************************/
void WiimoteYarp::run() {
    int command = pSharedArea->getCmd();
    if(command != VOCAB_NULL) {
        Bottle& miOutput = port.prepare();
        miOutput.clear();
        miOutput.addVocab(pSharedArea->getCmd());
        Bottle dBottle;
        dBottle.addDouble(pSharedArea->getRoll());
        dBottle.addDouble(pSharedArea->getPitch());
        miOutput.addList() = dBottle;
//        port.write(true);
        port.write(false);
        printf ("wrote (dir,oz,oy'): %s\n",miOutput.toString().c_str());
    }
}

/************************************************************************/
void WiimoteYarp::forceStop() {
    Bottle& miOutput = port.prepare();
    miOutput.clear();
    miOutput.addVocab(VOCAB_MY_STOP);
    port.write(true);
    printf ("wrote (dir,oz,oy'): %s\n",miOutput.toString().c_str());
}

