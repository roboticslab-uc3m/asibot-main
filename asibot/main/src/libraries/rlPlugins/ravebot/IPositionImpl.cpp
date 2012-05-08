
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
//    printf("RaveBot: setPositionMode().\n");
    if (modePosVel==0) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        printf("RaveBot::setPositionMode() return false; failed to stop\n");
        return false;
    }
    modePosVel = 0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(int j, double ref) {
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not positionMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::positionMove(%d,%f) f[begin]\n",j,ref);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=1;
    targetDeg[j]=ref;
    if (ref>realUnit[j]) jointVel[j] = refSpeed[j];
    else jointVel[j] = -refSpeed[j];
    printf("RaveBot::positionMove(%d,%f) f[end]\n",j,ref);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(const double *refs) {
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not positionMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::positionMove() f[begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    double time_max_dist = 0;
    for(unsigned int motor=0;motor<numMotors;motor++)
        if (fabs(refs[motor]-realUnit[motor])>max_dist) {
            max_dist = fabs(refs[motor]-realUnit[motor]);
            time_max_dist = max_dist/refSpeed[motor];  // the max_dist motor will be at refSpeed
        }
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(unsigned int motor=0;motor<numMotors;motor++) {
        joint_status[motor]=1;
        targetDeg[motor]=refs[motor];
        jointVel[motor] = (refs[motor]-realUnit[motor])/time_max_dist;
    }
    printf("RaveBot::positionMove() f[end]\n");
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(int j, double delta) {
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not relativeMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::relativeMove(%d,%f) f[begin]\n",j,delta);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=2;
    targetDeg[j]=realUnit[j]+delta;
    if (delta>0) jointVel[j] = refSpeed[j];
    else jointVel[j] = -refSpeed[j];
    printf("RaveBot::relativeMove(%d,%f) f[end]\n",j,delta);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(const double *deltas) {
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not relativeMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::relativeMove() f[begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    double time_max_dist = 0;
    for(int motor=0;motor<5;motor++)
        if (fabs(deltas[motor])>max_dist) {
            max_dist = fabs(deltas[motor]);
            time_max_dist = max_dist/refSpeed[motor];  // the max_dist motor will be at refSpeed
        }
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0; motor<5; motor++) {
      joint_status[motor]=2;
      targetDeg[motor]=realUnit[motor]+deltas[motor];
      jointVel[motor] = (deltas[motor])/time_max_dist;
    }
    printf("RaveBot::relativeMove() f[end]\n");
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

