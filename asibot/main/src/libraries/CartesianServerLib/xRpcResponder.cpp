
#include "xRpcResponder.h"

/************************************************************************/

bool xRpcResponder::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("[xRpcResponder] Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;
    int choice = in.get(0).asVocab();
    if (in.get(0).getCode() != BOTTLE_TAG_VOCAB) choice = VOCAB_FAILED;
    if (choice==VOCAB_HELP) {  ///////////////////////////////// help /////////////////////////////////
        out.addString("Available commands: [help] [inv] [movj] [movl] [stat] [stop] more info at [http://roboticslab.sourceforge.net/asibot/group__cartesianServer.html]");
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_MY_STOP) {  ///////////////////////////////// stop /////////////////////////////////
        if(icart->stopControl()) {
            if(ipos->stop()) {
                if(ipos->setPositionMode()) {
                    out.addVocab(VOCAB_OK);
                } else out.addVocab(VOCAB_FAILED);
            } else out.addVocab(VOCAB_FAILED);
        } else out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_STAT) { ///////////////////////////////// stat /////////////////////////////////
        Vector x,o;
        if(icart->getPose(x,o)) {
            Bottle& l1 = out.addList();
            for (int i = 0; i < x.size(); i++)
                l1.addDouble(x[i]);
            for (int i = 0; i < o.size(); i++)
                l1.addDouble(o[i]);
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_MOVL) { ///////////////////////////////// movl /////////////////////////////////
        Vector x,o;
        Bottle *lst = in.get(1).asList();
        x.push_back(lst->get(0).asDouble());
        x.push_back(lst->get(1).asDouble());
        x.push_back(lst->get(2).asDouble());
        for (int i = 3; i < lst->size(); i++)
            o.push_back(lst->get(i).asDouble());
        if(icart->goToPoseSync(x,o)){
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_MOVJ) { ///////////////////////////////// movj /////////////////////////////////
        Vector xd,od,xdhat,odhat,qdhat;
        Bottle *lst = in.get(1).asList();
        xd.push_back(lst->get(0).asDouble());
        xd.push_back(lst->get(1).asDouble());
        xd.push_back(lst->get(2).asDouble());
        for (int i = 3; i < lst->size(); i++)
            od.push_back(lst->get(i).asDouble());
        if(in.size()>2) {
            Bottle *toolLst = in.get(2).asList();
            printf("[not implemented] tool is list of %d elements\n", toolLst->size());
        }
        if(icart->askForPose(xd,od,xdhat,odhat,qdhat)){
            double qd[qdhat.size()];
            for (int i = 0; i < qdhat.size(); i++)
                qd[i] = qdhat[i];
            icart->stopControl(); // new!!!!
            ipos->setPositionMode();
            ipos->positionMove(qd);
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_INV) { ///////////////////////////////// inv /////////////////////////////////
        Vector xd,od,xdhat,odhat,qdhat;
        Bottle *lst = in.get(1).asList();
        xd.push_back(lst->get(0).asDouble());
        xd.push_back(lst->get(1).asDouble());
        xd.push_back(lst->get(2).asDouble());
        for (int i = 3; i < lst->size(); i++)
            od.push_back(lst->get(i).asDouble());
        if(icart->askForPose(xd,od,xdhat,odhat,qdhat)){
            Bottle& l1 = out.addList();
            for (int i = 0; i < qdhat.size(); i++)
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

void xRpcResponder::setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
    icart = _icart;
}

/************************************************************************/

void xRpcResponder::setPositionInterface(yarp::dev::IPositionControl* _ipos) {
    ipos = _ipos;
}

/************************************************************************/

