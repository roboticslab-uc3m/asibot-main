
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// ------------------- RateThread Related ------------------------------------

bool KdlBot::threadInit() {
    printf("[success] cartesianbot threadInit() started %f ms ratethread\n",getRate());
    double dRealUnits[100];
    if(!enc->getEncoders(dRealUnits)) {
        printf("[warning] KdlBot::threadInit() failed to getEncoders()\n");
        return false;  // bad practice??
    }
    yarp::sig::Vector realUnits(cmcNumMotors,dRealUnits);
    yarp::sig::Vector x,o;
    fwdKin(realUnits,x,o);
    printf("inside end of init\n");
    return true;
}

// -----------------------------------------------------------------------------

void KdlBot::run() {
    if (cmc_status>0) {  // If it is movement
        double dRealUnits[100];
        if(!enc->getEncoders(dRealUnits)) {
            printf("[warning] KdlBot::run() failed to getEncoders()\n");
            return;  // bad practice??
        }
        yarp::sig::Vector realUnits(cmcNumMotors,dRealUnits);
        yarp::sig::Vector x,o;
        fwdKin(realUnits,x,o);
        bool done = true;
        KDL::Frame currentF;
        currentF.p.data[0]=x[0];
        currentF.p.data[1]=x[1];
        currentF.p.data[2]=x[2];
        if (angleRepr == "eulerYZ") {  // ASIBOT
            currentF.M = Rotation::EulerZYZ(::atan2(x[1],x[0]),toRad(o[0]), toRad(o[1]));
        } else if (angleRepr == "eulerZYZ") {
            currentF.M = Rotation::EulerZYZ(toRad(o[0]), toRad(o[1]), toRad(o[2]));
        } else {  // axisAngle, etc.
            printf("[warning] KDL no compatible angleRepr\n");
        }

/*        for(int s=0;s<3;s++) {
            printf("fabs(x[%d]-targetF.p.data[%d]: %f\n",s,s,fabs(x[s]-targetF.p.data[s]));
            if(fabs(x[s]-targetF.p.data[s])>CARTPOS_PRECISION) done = false;
        }
        for(int s=0;s<o.size();s++) {
            printf("fabs(o[%d]-targetO[%d]: %f\n",s,s,fabs(o[s]-targetO[s]));
            if(fabs(o[s]-targetO[s])>CARTORI_PRECISION) done = false;
        }*/
        if(!Equal(currentF,targetF,epsilon)) done = false;
        
        if (done) {
            printf("Target reached in %f.\n",Time::now()-startTime);
            startTime = 0;
            pos->setPositionMode();
            cmc_status=0;
//            delete _orient;
//            _orient=0;
        } else {
            //printf("Inside control loop moving.\n");
 
/*            yarp::sig::Vector xP,xPd,xPdotd,eP,lawxP;
            lawxP.resize(3);
            lawxP = (eP * GAIN * (cmcMs/1000.0)) + xPdotd;  // GAIN=0 => lawxP = xPdotd;
            t = Ja_pinv * lawxP;*/

//            double _eps = 0.00001;
//            int _maxiter = 150;
//            ChainIkSolverVel_wdls iksolverv_wdls(theChain, _eps, _maxiter);
//            int ret_ik_v = iksolverv_wdls.CartToJnt(q_current,T_current,qdot_current);

            double sTime = Time::now()-startTime;
            if(sTime>currentTrajectory->Duration()){
                printf ("[warning] out of time at %f.\n",sTime);
                startTime = 0;
                pos->setPositionMode();
                cmc_status=0;
                return;  // bad practice??
            }

            KDL::Frame desiredF = currentTrajectory->Pos(sTime);
            KDL::Twist desiredT = currentTrajectory->Vel(sTime);

            JntArray currentRads = JntArray(cmcNumMotors);
            for (int motor=0; motor<cmcNumMotors; motor++) {
                if(isPrismatic[motor]) currentRads(motor)=dRealUnits[motor];
                else currentRads(motor)=toRad(dRealUnits[motor]);
            }

            KDL::Twist currentT = diff(currentF, desiredF);
            for(unsigned int i=0; i<6; i++) {
                currentT(i) *= GAIN;
                currentT(i) += desiredT(i);
            }

            JntArray kdlqdot = JntArray(cmcNumMotors);

            ChainIkSolverVel_pinv iksolverv(theChain);
            int ret = iksolverv.CartToJnt(currentRads,currentT,kdlqdot);

            double qdot[100];
            for (int motor=0; motor<cmcNumMotors; motor++) {
                if(isPrismatic[motor]) qdot[motor]=kdlqdot(motor);
                else qdot[motor]=toDeg(kdlqdot(motor));
            }
            if(!vel->velocityMove(qdot))
                printf("[kdlbot::run] GIGANTIC velocity WARNING\n");

        }
    } else {  // If it is stopped or breaked, reamain unchanged
        // printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

