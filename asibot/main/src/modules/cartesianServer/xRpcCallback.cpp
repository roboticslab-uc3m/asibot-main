
#include "xRpcCallback.h"

/************************************************************************/

bool xRpcCallback::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;  // Warning: unknown behaviour to j.
    int choice = in.get(0).asInt();
    if (in.get(0).getCode() != BOTTLE_TAG_INT) choice = -2;
    if (choice==-1) {  ///////////////////////////////// -1 /////////////////////////////////
        if(icart->stopControl())
            out.addVocab(VOCAB_OK);
        else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==0) { ///////////////////////////////// 0 /////////////////////////////////
        Vector x,o;
        if(icart->getPose(x,o)) {
            Bottle& l1 = out.addList();
            int i;
            for (i = 0; i < x.size(); i++)
                l1.addDouble(x[i]);
            for (i = 0; i < o.size(); i++)
                l1.addDouble(o[i]);
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==1) { ///////////////////////////////// 1 /////////////////////////////////
        Vector x,o;
        Bottle *lst = in.get(1).asList();
        printf("list of %d elements (5 needed: x y z oy' oz')\n", lst->size());
        if(lst->size() != 5) {
            out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return false;
        }
        x.push_back(lst->get(0).asDouble());
        x.push_back(lst->get(1).asDouble());
        x.push_back(lst->get(2).asDouble());
        o.push_back(lst->get(3).asDouble());
        o.push_back(lst->get(4).asDouble());
        if(icart->goToPose(x,o)){
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==15) { ///////////////////////////////// 15 /////////////////////////////////
        Vector xd,od,xdhat,odhat,qdhat;
        Bottle *lst = in.get(1).asList();
        printf("list of %d elements (5 needed: x y z oy' oz')\n", lst->size());
        if(lst->size() != 5) {
            out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return false;
        }
        xd.push_back(lst->get(0).asDouble());
        xd.push_back(lst->get(1).asDouble());
        xd.push_back(lst->get(2).asDouble());
        od.push_back(lst->get(3).asDouble());
        od.push_back(lst->get(4).asDouble());
        if(icart->askForPose(xd,od,xdhat,odhat,qdhat)){
            Bottle& l1 = out.addList();
            int i;
            for (i = 0; i < qdhat.size(); i++)
                l1.addDouble(qdhat[i]);
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    }
    out.addVocab(VOCAB_FAILED);
    out.write(*returnToSender);
    return false;
}

/************************************************************************/

void xRpcCallback::setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
    icart = _icart;
}

/************************************************************************/

void xRpcCallback::setPositionInterface(yarp::dev::IPositionControl* _ipos) {
    ipos = _ipos;
}

/************************************************************************/

