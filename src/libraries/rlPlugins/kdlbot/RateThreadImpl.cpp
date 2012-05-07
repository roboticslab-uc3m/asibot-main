
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// ------------------- RateThread Related ------------------------------------

bool KdlBot::threadInit() {
    printf("[success] cartesianbot threadInit() started %f ms ratethread\n",getRate());
    double dRealDeg[numMotors];
    if(!enc->getEncoders(dRealDeg)) {
        printf("[warning] KdlBot::threadInit() failed to getEncoders()\n");
        return false;  // bad practice??
    }
    yarp::sig::Vector realDeg(numMotors,dRealDeg);
    yarp::sig::Vector x,o;
    fwdKin(realDeg,x,o);
    printf("inside end of init\n");
    return true;
}

// -----------------------------------------------------------------------------

void KdlBot::run() {
    if (cmc_status>0) {  // If it is movement
        double dRealDeg[numMotors];
        if(!enc->getEncoders(dRealDeg)) {
            printf("[warning] KdlBot::run() failed to getEncoders()\n");
            return;  // bad practice??
        }
        yarp::sig::Vector realDeg(numMotors,dRealDeg);
        yarp::sig::Vector x,o;
        fwdKin(realDeg,x,o);
        bool done = true;
        if(fabs(x[0]-targetX[0])>CARTPOS_PRECISION) done = false;
        if(fabs(x[1]-targetX[1])>CARTPOS_PRECISION) done = false;
        if(fabs(x[2]-targetX[2])>CARTPOS_PRECISION) done = false;
        if((withOri)&&(fabs(o[0]-targetO[0])>CARTORI_PRECISION)) done = false;
        if((withOri)&&(fabs(o[1]-targetO[1])>CARTORI_PRECISION)) done = false;
        if (done) {
            printf("Target reached in %f.\n",Time::now()-startTime);
            startTime = 0;
            pos->setPositionMode();
            cmc_status=0;
        } else {
            //printf("Inside control loop moving.\n");
        }
    } else {  // If it is stopped or breaked, reamain unchanged
        // printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

