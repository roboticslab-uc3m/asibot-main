
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// ------------------- RateThread Related ------------------------------------

bool KdlBot::threadInit() {
    printf("[success] cartesianbot threadInit() started %f ms ratethread\n",getRate());
    double dRealUnits[numMotors];
    if(!enc->getEncoders(dRealUnits)) {
        printf("[warning] KdlBot::threadInit() failed to getEncoders()\n");
        return false;  // bad practice??
    }
    yarp::sig::Vector realUnits(numMotors,dRealUnits);
    yarp::sig::Vector x,o;
    fwdKin(realUnits,x,o);
    printf("inside end of init\n");
    return true;
}

// -----------------------------------------------------------------------------

void KdlBot::run() {
    if (cmc_status>0) {  // If it is movement
        double dRealUnits[numMotors];
        if(!enc->getEncoders(dRealUnits)) {
            printf("[warning] KdlBot::run() failed to getEncoders()\n");
            return;  // bad practice??
        }
        yarp::sig::Vector realUnits(numMotors,dRealUnits);
        yarp::sig::Vector x,o;
        fwdKin(realUnits,x,o);
        bool done = false;
        KDL::Vector currentX(x[0],x[1],x[2]);
        KDL::Rotation currentO;
        if (angleRepr == "eulerYZ") {  // ASIBOT
            KDL::Rotation currentO = Rotation::EulerZYZ(atan2(x[1],x[0]),toRad(o[0]), toRad(o[1]));
        } else if (angleRepr == "eulerZYZ") {
            KDL::Rotation currentO = Rotation::EulerZYZ(toRad(o[0]), toRad(o[1]), toRad(o[2]));
        } else {  // axisAngle, etc.
            printf("[warning] KDL no compatible angleRepr\n");
        }
        KDL::Frame currentF(currentO,currentX);

        if(KDL::Equal(currentF, targetF, DEFAULT_EPS)) done = true;
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

            ChainIkSolverVel_pinv iksolverv(theChain);

            JntArray currentRads = JntArray(numMotors);
            for (int motor=0; motor<numMotors; motor++) {
                if(isPrismatic[motor]) currentRads(motor)=dRealUnits[motor];
                else currentRads(motor)=toRad(dRealUnits[motor]);
            }
            KDL::Twist currentT = diff(currentF, targetF);
            JntArray kdlqdot = JntArray(numMotors);
            int ret = iksolverv.CartToJnt(currentRads,currentT,kdlqdot);

            double qdot[numMotors];
            for (int motor=0; motor<numMotors; motor++) {
                if(isPrismatic[motor]) qdot[motor]=kdlqdot(motor);
                else qdot[motor]=toDeg(kdlqdot(motor));
            }  
            if(!vel->velocityMove(qdot))
                printf("GIGANTIC velocity WARNING\n");

        }
    } else {  // If it is stopped or breaked, reamain unchanged
        // printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

