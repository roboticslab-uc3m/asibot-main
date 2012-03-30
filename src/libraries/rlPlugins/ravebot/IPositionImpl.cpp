
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------- IPositionControl Related --------------------------------

bool RaveBot::getAxes(int *ax) {
    *ax = numMotors;
    printf("RaveBot reporting %d axes are present\n", *ax);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setPositionMode() {
    printf("RaveBot: setPositionMode().\n");
    stop();
    modePosVel = 0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(int j, double ref) {
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::positionMove(%d,%f) [begin]\n",j,ref);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=1;
    targetDeg[j]=ref;
    if (ref>realDeg[j]) jointVel[j] = refSpeed[j];
    else jointVel[j] = -refSpeed[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(const double *refs) {
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::positionMove() [begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    for(unsigned int motor=0;motor<numMotors;motor++)
      if (fabs(refs[motor]-realDeg[motor])>max_dist)
        max_dist = fabs(refs[motor]-realDeg[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(unsigned int motor=0;motor<numMotors;motor++) {
      joint_status[motor]=1;
      targetDeg[motor]=refs[motor];
      jointVel[motor] = refSpeed[motor]*(refs[motor]-realDeg[motor])/max_dist;
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(int j, double delta) {
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::relativeMove(%d,%f) [begin]\n",j,delta);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=2;
    targetDeg[j]=realDeg[j]+delta;
    if (delta>0) jointVel[j] = refSpeed[j];
    else jointVel[j] = -refSpeed[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(const double *deltas) {
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::relativeMove() [begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    for(int motor=0;motor<5;motor++)
      if (fabs(deltas[motor])>max_dist)
        max_dist = fabs(deltas[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0; motor<5; motor++) {
      joint_status[motor]=2;
      targetDeg[motor]=realDeg[motor]+deltas[motor];
      jointVel[motor] = refSpeed[motor]*(deltas[motor])/max_dist;
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(int j, bool *flag) {
    if (joint_status[j]<=0) *flag=true;
    else *flag=false;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(bool *flag) {
    for (unsigned int i=0; i<numMotors; i++) {
      if (joint_status[i]<=0) flag[i]=true;
      else flag[i]=false;      
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeed(int j, double sp) {
    refSpeed[j]=sp;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeeds(const double *spds) {
    for (unsigned int i=0; i<numMotors; i++) {
        refSpeed[i]=spds[i];
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAcceleration(int j, double acc) {
    refAcc[j]=acc;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAccelerations(const double *accs) {
    for (unsigned int i=0; i<numMotors; i++) {
        refAcc[i]=accs[i];
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeed(int j, double *ref) {
    *ref=refSpeed[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeeds(double *spds) {
    for (unsigned int i=0; i<numMotors; i++)
      spds[i]=refSpeed[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAcceleration(int j, double *acc) {
    *acc=refAcc[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAccelerations(double *accs) {
    for (unsigned int i=0; i<numMotors; i++)
      accs[i]=refAcc[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop(int j) {
    jointVel[j]=0.0;
    joint_status[j]=-1;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop() {
    for (unsigned int i=0; i<numMotors; i++) {
      jointVel[i]=0.0;
      joint_status[i]=-1;
      theToolPort.status=0;
    }
    return true;
}

