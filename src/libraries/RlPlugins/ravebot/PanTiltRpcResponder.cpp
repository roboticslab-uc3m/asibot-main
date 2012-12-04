// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PanTiltRpcResponder.h"

/************************************************************************/

bool PanTiltRpcResponder::read(ConnectionReader& connection) {
    Bottle in, out;
    in.read(connection);
    printf("[PanTiltRpcResponder] Got %s\n", in.toString().c_str());
    out.clear();
    ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;
    //ConstString choice = in.get(0).asString();
    //if (in.get(0).getCode() != BOTTLE_TAG_STRING) choice="";
    if (in.get(0).asString()=="help") {  ///////////////////////////////// help /////////////////////////////////
        out.addString("Available commands: [help], [get] [encs], [set] [poss] (XX XX).");
        out.write(*returnToSender);
        return true;
    } else if ((in.get(0).asString()=="get")||(in.get(0).asVocab()==VOCAB_GET)) {
        Transform T = pPanTilt->GetTransform();
        printf("PanTilt robot at %f %f.\n",T.trans.x,T.trans.y);
        out.addVocab(VOCAB_IS);
        out.addString("encs");
        Bottle pose;
        pose.addDouble(T.trans.x);
        pose.addDouble(T.trans.y);
        out.addList() = pose;
        out.addVocab(VOCAB_OK);
        out.write(*returnToSender);
        return true;
    } else if ((in.get(0).asString()=="set")||(in.get(0).asVocab()==VOCAB_SET)) {
        Bottle* localtarget = in.get(2).asList();
        Transform T = pPanTilt->GetTransform();
        std::vector<dReal> v(3);
        if ((in.get(1).asString()=="poss")||(in.get(1).asVocab()==VOCAB_POSS)) {
            v[0] = localtarget->get(0).asDouble();
            v[1] = localtarget->get(1).asDouble();
            v[2] = localtarget->get(2).asDouble();
        } else if ((in.get(1).asString()=="rels")||(in.get(1).asVocab()==VOCAB_RELS)) {
            v[0] = T.trans.x + localtarget->get(0).asDouble();
            v[1] = T.trans.y + localtarget->get(1).asDouble();
            v[2] = localtarget->get(2).asDouble();
        } else {
            out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return true;
        }
        printf("PanTilt robot at %f %f, attempt to move to %f %f.\n",T.trans.x,T.trans.y,v[0],v[1]);
        out.addVocab(VOCAB_OK);
        out.write(*returnToSender);
        return true;
    }
    out.addVocab(VOCAB_FAILED);
    out.write(*returnToSender);
    return false;
}

/************************************************************************/

void PanTiltRpcResponder::setEnvironment(EnvironmentBasePtr _pEnv) {
    pEnv = _pEnv;
}

/************************************************************************/

void PanTiltRpcResponder::setPanTilt(RobotBasePtr _pPanTilt) {
    pPanTilt = _pPanTilt;
}

/************************************************************************/

void PanTiltRpcResponder::setEncRaw(const int Index, const double Position) {
    // printf("[SharedArea] setEncRaw.\n");
    encRawMutex.wait();
//    encRaw[Index] = Position;
    encRawMutex.post();
}

/************************************************************************/

double PanTiltRpcResponder::getEncRaw(const int Index) {
    // printf("[SharedArea] getEncRaw.\n");
    double Position;
    encRawMutex.wait();
//    Position = encRaw[Index];
    encRawMutex.post();
    return Position;
}

/************************************************************************/

double PanTiltRpcResponder::getEncExposed(const int Index) {
    double RawPosition;
    encRawMutex.wait();
//    RawPosition = encRaw[Index];
    encRawMutex.post();
    // printf("[SharedArea] get.\n");
//    return RawPosition / encRawExposed[Index];
    return 2;
}

/************************************************************************/

