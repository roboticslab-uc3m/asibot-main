
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
        if (choice==-1) {  ///////////////////////////////// x -1 /////////////////////////////////
            if(icart->stopControl())
                out.addVocab(VOCAB_OK);
            else
                out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return true;
        } else if (choice==0) { ///////////////////////////////// x 0 /////////////////////////////////
/*            if(icart->getPose())
                out.addVocab(VOCAB_OK);
            else
                out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return true;*/
        }
        out.addVocab(VOCAB_FAILED);
        if (returnToSender!=NULL) {
            out.write(*returnToSender);
        }
    }

/************************************************************************/
    
void xRpcCallback::setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
    icart = _icart;
}

/************************************************************************/

