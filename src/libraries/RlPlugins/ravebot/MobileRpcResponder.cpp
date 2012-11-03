// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "MobileRpcResponder.h"

/************************************************************************/

bool MobileRpcResponder::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("[MobileRpcResponder] Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;
    ConstString choice = in.get(0).asString();
    if (in.get(0).getCode() != BOTTLE_TAG_STRING) choice="";
    if (choice=="help") {  ///////////////////////////////// help /////////////////////////////////
        out.addString("Available commands: help, get encs, set pos XX XX.");
        out.write(*returnToSender);
        return true;
    } else if (choice=="get") {
        Transform T = pMobile->GetTransform();
        printf("Mobile robot at %f %f.\n",T.trans.x,T.trans.y);
        out.addVocab(VOCAB_IS);
        out.addString("encs");
        Bottle pose;
        pose.addDouble(T.trans.x);
        pose.addDouble(T.trans.y);
        out.addList() = pose;
        out.addVocab(VOCAB_OK);
        out.write(*returnToSender);
        return true;
    }
    out.addVocab(VOCAB_FAILED);
    out.write(*returnToSender);
    return false;
}

/************************************************************************/

void MobileRpcResponder::setEnvironment(EnvironmentBasePtr _pEnv) {
    pEnv = _pEnv;
}

/************************************************************************/

void MobileRpcResponder::setMobile(RobotBasePtr _pMobile) {
    pMobile = _pMobile;
}

/************************************************************************/

