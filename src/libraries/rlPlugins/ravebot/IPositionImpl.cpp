
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
    if (modePosVel==0) return true;  // Simply return true if we were already in pos mode.
    // Do anything additional before setting flag to pos...
    if(!stop()) {
        printf("RaveBot::setPositionMode() return false; failed to stop\n");
        return false;
    }
    modePosVel = 0;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::positionMove(int j, double ref) {  // encExposed = ref;
    if ((unsigned int)j>numMotors) return false;
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not positionMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::positionMove(%d,%f) f[begin]\n",j,ref);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    targetExposed[j] = ref;
    if (fabs(targetExposed[j]-getEncExposed(j))<jointTol[j]) {
        stop(j);  // puts jointStatus[j]=0;
        printf("Joint q%d reached target.\n",j+1);
        return true;
    } else if ( ref > getEncExposed(j) ) {
        //if(!velocityMove(j, refSpeed[j])) return false;
        velRaw[j] = (refSpeed[j] * velRawExposed[j]);
    } else {
        //if(!velocityMove(j, -refSpeed[j])) return false;
        velRaw[j] = -(refSpeed[j] * velRawExposed[j]);
    }
    jointStatus[j] = 1;
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
    // Find out the maximum time to move
    double max_time = 0;
    for(unsigned int motor=0;motor<numMotors;motor++) {
        printf("dist[%d]: %f\n",motor,fabs(refs[motor]-encRaw[motor]));
        printf("refSpeed[%d]: %f\n",motor,refSpeed[motor]);
//        printf("diff: %f\n",fabs(refs[motor]-encRaw[motor])/refSpeed[motor]);
        if (fabs((refs[motor]-encRaw[motor])/refSpeed[motor])>max_time) {
            max_time = fabs((refs[motor]-encRaw[motor])/refSpeed[motor]);  // the max_dist motor will be at refSpeed
            printf("-->candidate: %f\n",max_time);
        }
    }
    printf("max_time[final]: %f\n",max_time);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(unsigned int motor=0;motor<numMotors;motor++) {
        jointStatus[motor]=1;
        targetExposed[motor]=refs[motor];
        velRaw[motor] = (refs[motor]-encRaw[motor])/max_time;
        printf("velRaw[%d]: %f\n",motor,velRaw[motor]);
    }
    printf("RaveBot::positionMove() f[end]\n");
    return true;
}

//printf("max_dist[%d]: %f\n",motor,max_dist);
//printf("refSpeed[%d]: %f\n",motor,refSpeed[motor]);
//printf("time_max_dist[%d]: %f\n",motor,time_max_dist);
//printf("max_dist[final]: %f\n",max_dist);
//printf("time_max_dist[final]: %f\n",time_max_dist);

// -----------------------------------------------------------------------------

bool RaveBot::relativeMove(int j, double delta) {
    if ((unsigned int)j>numMotors) return false;
    if(modePosVel!=0) {  // Check if we are in position mode.
        printf("[fail] RaveBot will not relativeMove as not in positionMode\n");
        return false;
    }
    printf("RaveBot::relativeMove(%d,%f) f[begin]\n",j,delta);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    targetExposed[j]=getEncExposed(j)+delta;
    if (fabs(targetExposed[j]-getEncExposed(j))<jointTol[j]) {
        stop(j);  // puts jointStatus[j]=0;
        printf("Joint q%d reached target.\n",j+1);
        return true;
    } else if ( targetExposed[j] > getEncExposed(j) ) {
        // if(!velocityMove(j, refSpeed[j])) return false;
        velRaw[j] = (refSpeed[j] * velRawExposed[j]);
    } else {
        // if(!velocityMove(j, -refSpeed[j])) return false;
        velRaw[j] = -(refSpeed[j] * velRawExposed[j]);
    }
    jointStatus[j]=2;
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
      jointStatus[motor]=2;
      targetExposed[motor]=encRaw[motor]+deltas[motor];
      velRaw[motor] = (deltas[motor])/time_max_dist;
    }
    printf("RaveBot::relativeMove() f[end]\n");
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(int j, bool *flag) {
    if ((unsigned int)j>numMotors) return false;
    if (jointStatus[j]<=0) *flag=true;
    else *flag=false;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::checkMotionDone(bool *flag) {
    bool done = true;
    for (unsigned int i=0; i<numMotors; i++) {
        if (jointStatus[i]<=0) done = false;  // TODO: test, use "AND" if bad.
    }
    *flag = done;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeed(int j, double sp) {
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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
    if ((unsigned int)j>numMotors) return false;
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
    theToolPort.status=0;
    return ok;
}

// -----------------------------------------------------------------------------

