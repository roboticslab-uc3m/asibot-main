
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
        csStatus = 0;
    } else if (choice==VOCAB_FWD) { ///////////////////////////////// fwd /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xd,od;
        if(!icart->getPose(x,o)) return;
        double ozDeg = (atan2(x[1],x[0]))*180.0/M_PI;
        double PrP = sqrt(x[0]*x[0]+x[1]*x[1]);
        double PrPd = PrP + VPOINT_DIST*sin((cmd[1])*M_PI/180.0);
        printf("PrP: %f;PrPd: %f\n",PrP,PrPd);
        double PhPd = x[2] + VPOINT_DIST*cos((cmd[1])*M_PI/180.0);
        xd.push_back(PrPd*cos((ozDeg-cmd[0])*M_PI/180.0));  // xd
        xd.push_back(PrPd*sin((ozDeg-cmd[0])*M_PI/180.0));  // yd
        xd.push_back(PhPd);  // zd
        od.push_back(cmd[1]); // rot(y')d
        od.push_back(0.0); // rot(z'')d
        printf("xd: %s; od: %s\n",xd.toString().c_str(),od.toString().c_str());
        *csStatus = 3;
        icart->goToPose(xd,od,3.0);
    } else if (choice==VOCAB_BKWD) { ///////////////////////////////// bkwd /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xd,od;
        if(!icart->getPose(x,o)) {
            fprintf(stderr,"[CartesianServerLib] warning: getPose failed!\n");
            return;
        }
        double ozDeg = (atan2(x[1],x[0]))*180.0/M_PI;
        double PrP = sqrt(x[0]*x[0]+x[1]*x[1]);
        double PrPd = PrP - VPOINT_DIST*sin((cmd[1])*M_PI/180.0); 
        if (PrPd<=0) {
            printf("not reachable\n");
            return;
        }
        printf("PrP: %f;PrPd: %f\n",PrP,PrPd);
        double PhPd = x[2] - VPOINT_DIST*cos((cmd[1])*M_PI/180.0);
        xd.push_back(PrPd*cos((ozDeg-cmd[0])*M_PI/180.0));  // xd
        xd.push_back(PrPd*sin((ozDeg-cmd[0])*M_PI/180.0));  // yd
        xd.push_back(PhPd);  // zd
        od.push_back(cmd[1]); // rot(y')d
        od.push_back(0.0); // rot(z'')d
        printf("xd: %s; od: %s\n",xd.toString().c_str(),od.toString().c_str());
        *csStatus = 4;
        icart->goToPose(xd,od,3.0);
    } else if (choice==VOCAB_ROT) { ///////////////////////////////// rot /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        cmd.push_back(lst->get(0).asDouble());
        cmd.push_back(lst->get(1).asDouble());
        Vector x,o,xd,od;
        if(!icart->getPose(x,o)) {
            fprintf(stderr,"[CartesianServerLib] warning: getPose failed!\n");
            return;
        }
        double ozDeg = (atan2(x[1],x[0]))*180.0/M_PI;
        double PrP = sqrt(x[0]*x[0]+x[1]*x[1]);
        double PrPd = PrP;
        printf("PrP: %f;PrPd: %f\n",PrP,PrPd);
        double PhPd = x[2];
        xd.push_back(PrPd*cos((ozDeg-cmd[0])*M_PI/180.0));  // xd
        xd.push_back(PrPd*sin((ozDeg-cmd[0])*M_PI/180.0));  // yd
        xd.push_back(PhPd);  // zd
        od.push_back(cmd[1]); // rot(y')d
        od.push_back(0.0); // rot(z'')d
        printf("xd: %s; od: %s\n",xd.toString().c_str(),od.toString().c_str());
        *csStatus = 5;
        icart->goToPose(xd,od,3.0);
    } else if (choice==VOCAB_VMOS) { ///////////////////////////////// vmos /////////////////////////////////
        Bottle *lst = b.get(1).asList();
        Vector xdotd,odotd;
        if (b.size()==2) {  // no tool param, just send in base ref
            xdotd.push_back(lst->get(0).asDouble());
            xdotd.push_back(lst->get(1).asDouble());
            xdotd.push_back(lst->get(2).asDouble());
            for (int i = 3; i < lst->size(); i++)
                odotd.push_back(lst->get(i).asDouble());
        } else if (b.size()==3) {
            Vector x,o;
            if(!icart->getPose(x,o)) {
                fprintf(stderr,"[CartesianServerLib] warning: getPose failed!\n");
                return;
            }
            Vector oAA;  // axis-angle
            oAA.push_back(x[0]);
            oAA.push_back(x[1]);
            oAA.push_back(sqrt(x[0]*x[0]+x[1]*x[1])*cos(o[0]*M_PI/180.0));
            oAA.push_back((o[1])*M_PI/180.0);
            double length = sqrt(oAA[0]*oAA[0]+oAA[1]*oAA[1]+oAA[2]*oAA[2]);
            oAA[0] /= length;
            oAA[1] /= length;
            oAA[2] /= length;
            // Matrix axis2dcm(const Vector &v, unsigned int verbose);
            Matrix H_base_ee = axis2dcm(oAA,0);
            H_base_ee.resize(4,4);
            H_base_ee(0,3) = x[0];
            H_base_ee(1,3) = x[1];
            H_base_ee(2,3) = x[2];
            H_base_ee(3,3) = 1;
            Vector xdotdP;
            xdotdP.push_back(lst->get(0).asDouble());
            xdotdP.push_back(lst->get(1).asDouble());
            xdotdP.push_back(lst->get(2).asDouble());
            xdotdP.push_back(1);
            Vector xdotdPP = H_base_ee * xdotdP;
            xdotd.push_back(xdotdPP[0]);
            xdotd.push_back(xdotdPP[1]);
            xdotd.push_back(xdotdPP[2]);
            odotd.push_back(0);
            odotd.push_back(0);
        }
        printf("[xCallbackPort] xdotd: %s; odotd: %s\n",xdotd.toString().c_str(),odotd.toString().c_str());
        *csStatus = 6;
        icart->setTaskVelocities(xdotd,odotd);
    } else if (choice==VOCAB_POSE) { ///////////////////////////////// pose /////////////////////////////////
        Vector cmd;
        Bottle *lst = b.get(1).asList();
        Vector xd,od;
        xd.push_back(lst->get(0).asDouble());  // xd
        xd.push_back(lst->get(1).asDouble());  // xd
        xd.push_back(lst->get(2).asDouble());  // xd
        od.push_back(lst->get(3).asDouble());  // rot(y')d
        od.push_back(lst->get(4).asDouble());  // rot(z'')d
        //printf("xd: %s; od: %s\n",xd.toString().c_str(),od.toString().c_str());
        *csStatus = 7;
        icart->goToPose(xd,od,3.0);
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

void xCallbackPort::setCsStatus(int* _csStatus) {
    csStatus = _csStatus;
}

/************************************************************************/

