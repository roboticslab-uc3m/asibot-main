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
        out.addString("Available commands: help, world del all, world grab _obj_ _num_ 0/1, world mk sbox (three params for size) (three params for pos), world mk ssph (radius) (three params for pos), world mk scyl (radius height) (three params for pos).");
        out.write(*returnToSender);
        return true;
    } else if (choice=="world") {
        if (in.get(1).asString() == "mk") {
            if (in.get(2).asString() == "sbox") {
                { // lock the environment!           
                OpenRAVE::EnvironmentMutex::scoped_lock lock(pEnv->GetMutex());
                KinBodyPtr sboxKinBodyPtr = RaveCreateKinBody(pEnv,"");
                ConstString sboxName("sbox_");
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
            } if (in.get(2).asString() == "ssph") {
                { // lock the environment!           
                OpenRAVE::EnvironmentMutex::scoped_lock lock(pEnv->GetMutex());
                KinBodyPtr ssphKinBodyPtr = RaveCreateKinBody(pEnv,"");
                ConstString ssphName("ssph_");
                ssphName += ConstString::toString(ssphKinBodyPtrs.size()+1);
                ssphKinBodyPtr->SetName(ssphName.c_str());
                std::vector<Vector> spheres(1);
                spheres.push_back( Vector(in.get(4).asDouble(), in.get(5).asDouble(), in.get(6).asDouble(), in.get(3).asDouble() ));
                ssphKinBodyPtr->InitFromSpheres(spheres,true); 
                pEnv->Add(ssphKinBodyPtr,true);
                ssphKinBodyPtrs.push_back(ssphKinBodyPtr);
                }  // the environment is not locked anymore
                out.addVocab(VOCAB_OK);
            } if (in.get(2).asString() == "scyl") {
                { // lock the environment!           
                OpenRAVE::EnvironmentMutex::scoped_lock lock(pEnv->GetMutex());
                KinBodyPtr scylKinBodyPtr = RaveCreateKinBody(pEnv,"");
                ConstString scylName("scyl_");
                scylName += ConstString::toString(scylKinBodyPtrs.size()+1);
                scylKinBodyPtr->SetName(scylName.c_str());
                std::list<KinBody::Link::GeometryInfo> scylInfoList;
                KinBody::Link::GeometryInfo scylInfo;
                scylInfo._type = KinBody::Link::GeomCylinder;
                Transform pose(Vector(1,0,0,0),Vector(in.get(5).asDouble(),in.get(6).asDouble(),in.get(7).asDouble()));
                scylInfo._t = pose;  // scylInfo._t[0,3] = 0.1;
                Vector volume;
                volume.x = in.get(3).asDouble();
                volume.y = in.get(4).asDouble();
                scylInfo._vGeomData = volume;
                scylInfo._bVisible = true;
                //scylInfo._fTransparency = 0.5;
                //scylInfo._vDiffuseColor = [1,0,0];
                scylInfoList.push_back(scylInfo);
                scylKinBodyPtr->InitFromGeometries(scylInfoList);
                pEnv->Add(scylKinBodyPtr,true);
                scylKinBodyPtrs.push_back(scylKinBodyPtr);
                }  // the environment is not locked anymore
                out.addVocab(VOCAB_OK);
            } else out.addVocab(VOCAB_FAILED);
        } else if ((in.get(1).asString()=="del")&&(in.get(2).asString()=="all")) {
            for (unsigned int i=0;i<sboxKinBodyPtrs.size();i++) {
                pEnv->Remove(sboxKinBodyPtrs[i]);
            }
            sboxKinBodyPtrs.clear();
            for (unsigned int i=0;i<ssphKinBodyPtrs.size();i++) {
                pEnv->Remove(ssphKinBodyPtrs[i]);
            }
            ssphKinBodyPtrs.clear();
            for (unsigned int i=0;i<scylKinBodyPtrs.size();i++) {
                pEnv->Remove(scylKinBodyPtrs[i]);
            }
            scylKinBodyPtrs.clear();
            out.addVocab(VOCAB_OK);
        } else if (in.get(1).asString()=="grab") {
            if(in.get(2).asString()=="sbox") {
                int inIndex = (in.get(3).asInt());
                if ( (inIndex>=1) && (inIndex<=(int)sboxKinBodyPtrs.size()) ) {
                    if (in.get(4).asInt()==1) {
                        pRobot->Grab(sboxKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else if (in.get(4).asInt()==0) {
                        pRobot->Release(sboxKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else out.addVocab(VOCAB_FAILED);
                } else out.addVocab(VOCAB_FAILED);
            } else if(in.get(2).asString()=="ssph") {
                int inIndex = (in.get(3).asInt());
                if ( (inIndex>=1) && (inIndex<=(int)ssphKinBodyPtrs.size()) ) {
                    if (in.get(4).asInt()==1) {
                        pRobot->Grab(ssphKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else if (in.get(4).asInt()==0) {
                        pRobot->Release(ssphKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else out.addVocab(VOCAB_FAILED);
                } else out.addVocab(VOCAB_FAILED);
            } else if(in.get(2).asString()=="scyl") {
                int inIndex = (in.get(3).asInt());
                if ( (inIndex>=1) && (inIndex<=(int)scylKinBodyPtrs.size()) ) {
                    if (in.get(4).asInt()==1) {
                        pRobot->Grab(scylKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else if (in.get(4).asInt()==0) {
                        pRobot->Release(scylKinBodyPtrs[inIndex-1]);
                        out.addVocab(VOCAB_OK);
                    } else out.addVocab(VOCAB_FAILED);
                } else out.addVocab(VOCAB_FAILED);
            } else out.addVocab(VOCAB_FAILED);
        } else out.addVocab(VOCAB_FAILED);
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

