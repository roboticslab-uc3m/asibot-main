// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WorldRpcResponder.h"

/************************************************************************/

bool WorldRpcResponder::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("[WorldRpcResponder] Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;
    ConstString choice = in.get(0).asString();
    if (in.get(0).getCode() != BOTTLE_TAG_STRING) choice="";
    if (choice=="help") {  ///////////////////////////////// help /////////////////////////////////
        out.addString("Available commands: help, world mk box (three params for size) (three params for pos).");
        out.write(*returnToSender);
        return true;
    } else if (choice=="world") {
        if (in.get(1).asString() == "mk") {
            if (in.get(2).asString() == "sbox") {
                { // lock the environment!           
                OpenRAVE::EnvironmentMutex::scoped_lock lock(pEnv->GetMutex());
                KinBodyPtr sboxKinBodyPtr = RaveCreateKinBody(pEnv,"");
                ConstString sboxName("sbox");
                sboxName += ConstString::toString(sboxKinBodyPtrs.size()+1);
                sboxKinBodyPtr->SetName(sboxName.c_str());
                std::vector<AABB> boxes(1);
                boxes[0].extents = Vector(in.get(3).asDouble(), in.get(4).asDouble(), in.get(5).asDouble());
                boxes[0].pos = Vector(in.get(6).asDouble(), in.get(7).asDouble(), in.get(8).asDouble());
                sboxKinBodyPtr->InitFromBoxes(boxes,true); 
                pEnv->Add(sboxKinBodyPtr,true);
                sboxKinBodyPtrs.push_back(sboxKinBodyPtr);
                }  // the environment is not locked anymore
                out.addVocab(VOCAB_OK);
            } else out.addVocab(VOCAB_FAILED);
        } else if ((in.get(1).asString()=="del")&&(in.get(2).asString()=="all")) {
            for (unsigned int i=0;i<sboxKinBodyPtrs.size();i++) {
                pEnv->Remove(sboxKinBodyPtrs[i]);
            }
            sboxKinBodyPtrs.clear();
            out.addVocab(VOCAB_OK);
        } else if (in.get(1).asString()=="grab") {
            if (in.get(2).asInt()==1) {
                //pRobot->Grab(kinBodyPtr);
                out.addVocab(VOCAB_OK);
            } else if (in.get(2).asInt()==0) {
                //pRobot->Release(kinBodyPtr);
                out.addVocab(VOCAB_OK);
            } else out.addVocab(VOCAB_FAILED);
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

void WorldRpcResponder::setEnvironment(EnvironmentBasePtr _pEnv) {
    pEnv = _pEnv;
}

/************************************************************************/

void WorldRpcResponder::setRobot(RobotBasePtr _pRobot) {
    pRobot = _pRobot;
}

/************************************************************************/

