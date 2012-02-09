
#include "xCallbackPort.h"

/************************************************************************/

void xCallbackPort::onRead(Bottle& b) {
    printf("[xCallbackPort] Got %s\n", b.toString().c_str());
    int choice = b.get(0).asVocab();
    if (b.get(0).getCode() != BOTTLE_TAG_VOCAB) choice = VOCAB_FAILED;
    if (choice==VOCAB_MY_STOP) {  ///////////////////////////////// stop /////////////////////////////////
        icart->stopControl();
        ipos->stop();
        ipos->setPositionMode();
    } else if (choice==VOCAB_FWD) { ///////////////////////////////// fwd /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        printf("FWD list of %d elements (2 needed: oz oy')\n", lst->size());
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xv,ov;
        if(!icart->getPose(x,o)) return;
        double ozRad = atan2(x[1],x[0]);
        double PrP = x[0]*x[0]+x[1]*x[1];
        xv.push_back(x[0]);
        xv.push_back(x[1]);
        xv.push_back(x[2]);
        ov.push_back(o[0]);
        ov.push_back(0.0);
        icart->goToPose(xv,ov);
    } else if (choice==VOCAB_BKWD) { ///////////////////////////////// bkwd /////////////////////////////////
        Bottle *lst = b.get(1).asList();
        printf("BKWD list of %d elements (2 needed: oz oy')\n", lst->size());
        Vector cmd;
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xv,ov;
        if(!icart->getPose(x,o)) return;
        double ozRad = atan2(x[1],x[0]);
        double PrP = x[0]*x[0]+x[1]*x[1];
        xv.push_back(x[0]);
        xv.push_back(x[1]);
        xv.push_back(x[2]);
        ov.push_back(o[0]);
        ov.push_back(0.0);
        icart->goToPose(xv,ov);
    }
}

/************************************************************************/

void xCallbackPort::setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
    icart = _icart;
}

/************************************************************************/

void xCallbackPort::setPositionInterface(yarp::dev::IPositionControl* _ipos) {
    ipos = _ipos;
}

/************************************************************************/

