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
    } else if (choice=="set") {
        Bottle* localtarget = in.get(2).asList();
        Transform T = pMobile->GetTransform();
        std::vector<dReal> v(2);
        if (in.get(1).asString()=="poss") {
            v[0] = localtarget->get(0).asDouble();
            v[1] = localtarget->get(1).asDouble();
        } else if (in.get(1).asString()=="rels") {
            v[0] = T.trans.x + localtarget->get(0).asDouble();
            v[1] = T.trans.y + localtarget->get(1).asDouble();
        } else {
            out.addVocab(VOCAB_FAILED);
            out.write(*returnToSender);
            return true;
        }
        printf("Mobile robot at %f %f, attempt to move to %f %f.\n",T.trans.x,T.trans.y,v[0],v[1]);
        {
            EnvironmentMutex::scoped_lock lock(pEnv->GetMutex()); // lock environment
            // find a set of free joint values for the robot
            {
                RobotBase::RobotStateSaver saver(pMobile); // save the state
                while(1) {
                    pMobile->SetActiveDOFValues(v);
                    if( !pEnv->CheckCollision(pMobile) && !pMobile->CheckSelfCollision() ) {
                        break;
                    }
                }
                // robot state is restored
            }
            std::stringstream cmdin,cmdout;
            cmdin << "MoveActiveJoints goal ";
            for(size_t i = 0; i < v.size(); ++i) {
                cmdin << v[i] << " ";
            }
            // start the planner and run the robot
            RAVELOG_INFO("%s\n",cmdin.str().c_str());
            if( !pModule->SendCommand(cmdout,cmdin) ) {
                out.addVocab(VOCAB_FAILED);
                out.write(*returnToSender);
                return true;
                //continue;
            }
        }
        
        printf("Unlock the environment and wait for the mobile robot to finish...\n");
        while(!pMobile->GetController()->IsDone()) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
        printf("mobile robot finished.\n");
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

void MobileRpcResponder::setModule(ModuleBasePtr _pModule) {
    pModule = _pModule;
}

/************************************************************************/

