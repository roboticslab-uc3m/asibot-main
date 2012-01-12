
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------- IPositionControl Related --------------------------------

bool RaveBot::getAxes(int *ax) {
    *ax = NUM_MOTORS;
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
    target_degrees[j]=ref;
    if (ref>real_degrees[j]) joint_vel[j] = (THREAD_RATE*SPEED_ADJ*gvels[j])/100.0;
    else joint_vel[j] = -(THREAD_RATE*SPEED_ADJ*gvels[j])/100.0;
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
    for(int motor=0;motor<NUM_MOTORS;motor++)
      if (fabs(refs[motor]-real_degrees[motor])>max_dist)
        max_dist = fabs(refs[motor]-real_degrees[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0;motor<NUM_MOTORS;motor++) {
      joint_status[motor]=1;
      target_degrees[motor]=refs[motor];
      joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(refs[motor]-real_degrees[motor])/max_dist)/(100.0);
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
    target_degrees[j]=real_degrees[j]+delta;
    if (delta>0) joint_vel[j] = (THREAD_RATE*SPEED_ADJ*gvels[j])/(100.0);
    else joint_vel[j] = -(THREAD_RATE*SPEED_ADJ*gvels[j])/(100.0);
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
      target_degrees[motor]=real_degrees[motor]+deltas[motor];
      joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(deltas[motor])/max_dist)/(100.0);
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
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      if (joint_status[i]<=0) flag[i]=true;
      else flag[i]=false;      
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeed(int j, double sp) {
    if (sp>100) gvels[j]=100;
    else if (sp<-100) gvels[j]=-100;
    else gvels[j]=sp;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefSpeeds(const double *spds) {
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      if (spds[i]>100) gvels[i]=100;
      else if (spds[i]<-100) gvels[i]=-100;
      else gvels[i]=spds[i];
    }
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAcceleration(int j, double acc) {
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::setRefAccelerations(const double *accs) {
    return true;  // Not implemented, its unlimited for now
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeed(int j, double *ref) {
    *ref=gvels[j];   
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefSpeeds(double *spds) {
    for (unsigned int i=0; i<NUM_MOTORS; i++)
      spds[i]=gvels[i];
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAcceleration(int j, double *acc) {
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::getRefAccelerations(double *accs) {
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop(int j) {
    joint_vel[j]=0.0;
    joint_status[j]=-1;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::stop() {
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      joint_vel[i]=0.0;
      joint_status[i]=-1;
      theToolPort.status=0;
    }
    return true;
}

