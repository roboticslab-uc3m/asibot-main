
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
        Vector x,o,xd,od;
        if(!icart->getPose(x,o)) return;
//        double ozRad = atan2(x[1],x[0]);
        double PrP = x[0]*x[0]+x[1]*x[1];
        double PrPd = PrP + VPOINT_DIST*sin((cmd[1])*M_PI/180.0);
        double PhPd = x[2] + VPOINT_DIST*cos((cmd[1])*M_PI/180.0);
        xd.push_back(PrPd*cos((cmd[0])*M_PI/180.0));  // xd
        xd.push_back(PrPd*sin((cmd[0])*M_PI/180.0));  // yd
        xd.push_back(PhPd);  // zd
        od.push_back(cmd[1]); // rot(y')d
        od.push_back(0.0); // rot(z'')d
        icart->goToPose(xd,od);
    } else if (choice==VOCAB_BKWD) { ///////////////////////////////// bkwd /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        printf("BKWD list of %d elements (2 needed: oz oy')\n", lst->size());
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xd,od;
        if(!icart->getPose(x,o)) return;
//        double ozRad = atan2(x[1],x[0]);
        double PrP = x[0]*x[0]+x[1]*x[1];
        double PrPd = PrP - VPOINT_DIST*sin((cmd[1])*M_PI/180.0); 
        double PhPd = x[2] - VPOINT_DIST*cos((cmd[1])*M_PI/180.0);
        xd.push_back(PrPd*cos((cmd[0])*M_PI/180.0));  // xd
        xd.push_back(PrPd*sin((cmd[0])*M_PI/180.0));  // yd
        xd.push_back(PhPd);  // zd
        od.push_back(cmd[1]); // rot(y')d
        od.push_back(0.0); // rot(z'')d
        icart->goToPose(xd,od);
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

