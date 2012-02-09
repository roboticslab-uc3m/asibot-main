
#include "xCallbackPort.h"

/************************************************************************/

void xCallbackPort::onRead(Bottle& b) {
    printf("[xCallbackPort] Got %s\n", b.toString().c_str());
    int choice = b.get(0).asVocab();
    if (b.get(0).getCode() != BOTTLE_TAG_VOCAB) choice = VOCAB_FAILED;
    if (choice==VOCAB_MY_STOP) {  ///////////////////////////////// stop /////////////////////////////////
        icart->stopControl();
        ipos->stop();
    } else if (choice==VOCAB_FWD) { ///////////////////////////////// fwd /////////////////////////////////
        Vector x,o;
        Bottle *lst = b.get(1).asList();
        printf("FWD list of %d elements (2 needed: oz oy')\n", lst->size());
        x.push_back(lst->get(0).asDouble());
        x.push_back(lst->get(1).asDouble());
//        icart->goToPose(x,o);
    } else if (choice==VOCAB_BKWD) { ///////////////////////////////// bkwd /////////////////////////////////
        Vector x,o;
        Bottle *lst = b.get(1).asList();
        printf("BKWD list of %d elements (2 needed: oz oy')\n", lst->size());
        x.push_back(lst->get(0).asDouble());
        x.push_back(lst->get(1).asDouble());
//        icart->goToPose(x,o);
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

