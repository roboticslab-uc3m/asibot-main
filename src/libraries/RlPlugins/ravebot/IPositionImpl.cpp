// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------- IPositionControl Related --------------------------------

bool RaveBot::getAxes(int *ax) {
    *ax = numMotors;
    printf("[RaveBot] getAxes reporting %d axes are present\n", *ax);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(int j, double ref) {  // encExposed = ref;
    if (!indexWithinRange(j)) return false;
    if(vModePosVel[j]!=VOCAB_CM_POSITION) {  // Check if we are in position mode.
        fprintf(stderr,"[RaveBot] warning: will not positionMove as joint %d not in positionMode\n",j+1);
        return false;
    }
    printf("[RaveBot] positionMove(%d,%f) f[begin]\n",j,ref);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    targetExposed[j] = ref;
    if (fabs(targetExposed[j]-getEncExposed(j))<jointTol[j]) {
        stop(j);  // puts jointStatus[j]=0;
        printf("[RaveBot] Joint q%d reached target.\n",j+1);
        return true;
    } else if ( ref > getEncExposed(j) ) {
        //if(!velocityMove(j, refSpeed[j])) return false;
        velRaw[j] = (refSpeed[j] * velRawExposed[j]);
    } else {
        //if(!velocityMove(j, -refSpeed[j])) return false;
        velRaw[j] = -(refSpeed[j] * velRawExposed[j]);
    }
    jointStatus[j] = 1;
    printf("[RaveBot] positionMove(%d,%f) f[end]\n",j,ref);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(const double *refs) {  // encExposed = refs;
    for(unsigned int motor=0;motor<numMotors;motor++) {
        if(vModePosVel[motor]!=VOCAB_CM_POSITION) {  // Check if we are in position mode.
            fprintf(stderr,"[RaveBot] error: Will not positionMove as joint %d not in positionMode\n",motor+1);
            return false;
        }
    }
    printf("[RaveBot] positionMove() f[begin]\n");
    // Find out the maximum time to move
    double max_time = 0;
    for(unsigned int motor=0;motor<numMotors;motor++) {
        printf("[RaveBot] dist[%d]: %f\n",motor,fabs(refs[motor]-getEncExposed(motor)));
        printf("[RaveBot] refSpeed[%d]: %f\n",motor,refSpeed[motor]);
        if (fabs((refs[motor]-getEncExposed(motor))/refSpeed[motor])>max_time) {
            max_time = fabs((refs[motor]-getEncExposed(motor))/refSpeed[motor]);
            printf("[RaveBot] -->candidate: %f\n",max_time);
        }
    }
    printf("[RaveBot] max_time[final]: %f\n",max_time);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(unsigned int motor=0;motor<numMotors;motor++) {
        targetExposed[motor]=refs[motor];
        velRaw[motor] = ((refs[motor]-getEncExposed(motor))/max_time)*velRawExposed[motor];
        //if(velRaw[motor] != velRaw[motor]) velRaw[motor] = 0;  // protect against NaN
        printf("[RaveBot] velRaw[%d]: %f\n",motor,velRaw[motor]);
        jointStatus[motor]=1;
        if (fabs(targetExposed[motor]-getEncExposed(motor))<jointTol[motor]) {
            stop(motor);  // puts jointStatus[motor]=0;
            printf("[RaveBot] Joint q%d reached target.\n",motor+1);
        }
    }
    printf("[RaveBot] positionMove() f[end]\n");
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(int j, double delta) {
    if (!indexWithinRange(j)) return false;
    if(vModePosVel[j]!=VOCAB_CM_POSITION) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not relativeMove as joint %d not in positionMode\n",j+1);
        return false;
    }
    printf("[RaveBot] relativeMove(%d,%f) f[begin]\n",j,delta);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    targetExposed[j]=getEncExposed(j)+delta;
    if (fabs(targetExposed[j]-getEncExposed(j))<jointTol[j]) {
        stop(j);  // puts jointStatus[j]=0;
        printf("[RaveBot] Joint q%d already at target.\n",j+1);
        return true;
    } else if ( targetExposed[j] > getEncExposed(j) ) {
        // if(!velocityMove(j, refSpeed[j])) return false;
        velRaw[j] = (refSpeed[j] * velRawExposed[j]);
    } else {
        // if(!velocityMove(j, -refSpeed[j])) return false;
        velRaw[j] = -(refSpeed[j] * velRawExposed[j]);
    }
    jointStatus[j]=2;
    printf("[RaveBot] relativeMove(%d,%f) f[end]\n",j,delta);
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(const double *deltas) {  // encExposed = deltas + encExposed
    for(unsigned int motor=0;motor<numMotors;motor++) {
        if(vModePosVel[motor]!=VOCAB_CM_POSITION) {  // Check if we are in position mode.
            fprintf(stderr,"[RaveBot] warning: will not relativeMove as joint %d not in positionMode\n",motor+1);
            return false;
        }
    }
    printf("[RaveBot] relativeMove() f[begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    double time_max_dist = 0;
    for(unsigned int motor=0;motor<numMotors;motor++)
        if (fabs(deltas[motor])>max_dist) {
            max_dist = fabs(deltas[motor]);
            time_max_dist = max_dist/refSpeed[motor];  // the max_dist motor will be at refSpeed
        }
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(unsigned int motor=0; motor<numMotors; motor++) {
      targetExposed[motor]=getEncExposed(motor)+deltas[motor];
      velRaw[motor] = ((deltas[motor])/time_max_dist)*velRawExposed[motor];
      printf("velRaw[%d]: %f\n",motor,velRaw[motor]);
      jointStatus[motor]=2;
    }
    printf("[RaveBot] relativeMove() f[end]\n");
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(int j, bool *flag) {
    if (!indexWithinRange(j)) return false;
    bool done = true;
    if (jointStatus[j]>0) done=false;
    *flag = done;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(bool *flag) {
    bool done = true;
    for (unsigned int i=0; i<numMotors; i++) {
        if (jointStatus[i]>0) done = false;
    }
    *flag = done;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeed(int j, double sp) {
    if (!indexWithinRange(j)) return false;
    refSpeed[j]=sp;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeeds(const double *spds) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= setRefSpeed(i,spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAcceleration(int j, double acc) {
    if (!indexWithinRange(j)) return false;
    refAcc[j]=acc;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAccelerations(const double *accs) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= setRefAcceleration(i,accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeed(int j, double *ref) {
    if (!indexWithinRange(j)) return false;
    *ref=refSpeed[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeeds(double *spds) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getRefSpeed(i,&spds[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAcceleration(int j, double *acc) {
    if (!indexWithinRange(j)) return false;
    *acc=refAcc[j];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAccelerations(double *accs) {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= getRefAcceleration(i,&accs[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop(int j) {
    if (!indexWithinRange(j)) return false;
    printf("[RaveBot] stop(%d)\n",j);
    velRaw[j]=0.0;
    jointStatus[j]=0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop() {
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= stop(i);
    return ok;
}

// -----------------------------------------------------------------------------

