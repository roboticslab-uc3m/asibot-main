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
        out.addString("Available commands: [help] [run] more info at [http://roboticslab.sourceforge.net/asibot/group__cartesianServer.html]");
        return out.write(*returnToSender);
    } else if ((in.get(0).asString() == "run")||(in.get(0).asVocab() == VOCAB_RUN)) {  // run //
        if (in.get(1).asString() == "grab") {
            Property options;
            options.put("robotAngle",30.0);
            taskGrabCanSim.configure(options);
            if(taskGrabCanSim.run() ) {
                printf("[TaskRpcResponder] success: taskGrabCanSim.run()\n");
                out.addVocab(VOCAB_OK);
            } else {
                fprintf(stderr,"[TaskRpcResponder] failed: taskGrabCanSim.run()\n");
                out.addVocab(VOCAB_FAILED);
            }
            taskGrabCanSim.close();
        } else {
            fprintf(stderr,"[TaskRpcResponder] failed: I don't know how to run that.\n");
            out.addVocab(VOCAB_FAILED);
        }
        return out.write(*returnToSender);
    } else {
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

