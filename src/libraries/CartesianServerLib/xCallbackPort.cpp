
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
        } else if (b.size()==3) {
            Vector x,o;
            if(!icart->getPose(x,o)) {
                fprintf(stderr,"[CartesianServerLib] warning: getPose failed!\n");
                return;
            }
            // Matrix axis2dcm(const Vector &v, unsigned int verbose);
            Matrix H_base_ee = axis2dcm(o,0);
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
        }
        for (int i = 3; i < lst->size(); i++)
            odotd.push_back(lst->get(i).asDouble());
        printf("xdotd: %s; odotd: %s\n",xdotd.toString().c_str(),odotd.toString().c_str());
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

Matrix xCallbackPort::axis2dcm(const Vector &v, unsigned int verbose) {

    if (v.length()<4) {
        if (verbose)
            fprintf(stderr,"axis2dcm() failed\n");
    
        return Matrix(0,0);
    }

    Matrix R=eye(4,4);

    double theta=v[3];
    if (theta==0.0)
        return R;

    double c=cos(theta);
    double s=sin(theta);
    double C=1.0-c;

    double xs =v[0]*s;
    double ys =v[1]*s;
    double zs =v[2]*s;
    double xC =v[0]*C;
    double yC =v[1]*C;
    double zC =v[2]*C;
    double xyC=v[0]*yC;
    double yzC=v[1]*zC;
    double zxC=v[2]*xC;
    
    R(0,0)=v[0]*xC+c;
    R(0,1)=xyC-zs;
    R(0,2)=zxC+ys;
    R(1,0)=xyC+zs;
    R(1,1)=v[1]*yC+c;
    R(1,2)=yzC-xs;
    R(2,0)=zxC-ys;
    R(2,1)=yzC+xs;
    R(2,2)=v[2]*zC+c;

    return R;
}

/************************************************************************/

