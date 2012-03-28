
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
    } else if (choice==VOCAB_MOVL) { ///////////////////////////////// movl /////////////////////////////////
        Vector x,o;
        Bottle *lst = in.get(1).asList();
        printf("list of %d elements (5 needed: x y z oy' oz'')\n", lst->size());
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
        if(icart->goToPoseSync(x,o)){
            out.addVocab(VOCAB_OK);
        } else
            out.addVocab(VOCAB_FAILED);
        out.write(*returnToSender);
        return true;
    } else if (choice==VOCAB_MOVJ) { ///////////////////////////////// movj /////////////////////////////////
        Vector xd,od,xdhat,odhat,qdhat;
        Bottle *lst = in.get(1).asList();
        printf("target is list of %d elements (5 needed: x y z oy' oz'')\n", lst->size());
        if(lst->size() != 5) {
            out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return false;
        }
        int args = in.size();
        xd.push_back(lst->get(0).asDouble());
        xd.push_back(lst->get(1).asDouble());
        xd.push_back(lst->get(2).asDouble());
        od.push_back(lst->get(3).asDouble());
        od.push_back(lst->get(4).asDouble());
        if(args>2) {
            Bottle *toolLst = in.get(2).asList();
            printf("tool is list of %d elements (5 needed: x y z oy' oz'')\n", toolLst->size());
        } else printf("Got NO tool arg.\n");
        if(icart->askForPose(xd,od,xdhat,odhat,qdhat)){
            double qd[qdhat.size()];
            for (int i = 0; i < qdhat.size(); i++)
                qd[i] = qdhat[i];
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
        printf("list of %d elements (5 needed: x y z oy' oz'')\n", lst->size());
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

