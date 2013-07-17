// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskRpcResponder.hpp"

/************************************************************************/

bool TaskRpcResponder::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("[TaskRpcResponder] Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;
    if ((in.get(0).asString() == "help")||(in.get(0).asVocab() == VOCAB_HELP)) {  // help //
        out.addString("Available commands: [help] [inv] [movj] [movl] [stat] [stop] [wait] [tool] more info at [http://roboticslab.sourceforge.net/asibot/group__cartesianServer.html]");
        return out.write(*returnToSender);
    } /*else if ((in.get(0).asString() == "stop")||(in.get(0).asVocab() == VOCAB_STOP)) {  // stop //
        if(icart->stopControl()) {
            if(ipos->stop()) {
                if(ipos->setPositionMode()) {
                    out.addVocab(VOCAB_OK);
                    *tsStatus = 0;
                } else out.addVocab(VOCAB_FAILED);
            } else out.addVocab(VOCAB_FAILED);
        } else out.addVocab(VOCAB_FAILED);
        return out.write(*returnToSender);
    } */ else {
        fprintf(stderr,"[TaskRpcResponder] fail: Unknown command (use 'help' if needed).\n");
        out.addVocab(VOCAB_FAILED);
        return out.write(*returnToSender);
    }
}

/************************************************************************/

void TaskRpcResponder::setTsStatus(int* _tsStatus) {
    tsStatus = _tsStatus;
}

/************************************************************************/

