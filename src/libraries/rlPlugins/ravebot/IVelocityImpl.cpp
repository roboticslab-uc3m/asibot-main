
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IVelocity Related ----------------------------------------

bool RaveBot::setVelocityMode() {
    printf("RaveBot::setVelocityMode()\n");
    modePosVel = 1;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(int j, double sp) {
    if(sp>0) target_degrees[j]=180.0; // Must correct for JL
    else target_degrees[j]=-180.0;
    if (sp>100) sp=100;
    else if (sp<-100) sp=-100;
//    gvels[j]=sp;
//    joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(deltas[motor])/max_dist)/(100.0);
    joint_vel[j] = (THREAD_RATE*SPEED_ADJ_V*sp)/(100.0);
    joint_status[j]=3;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(const double *sp) {
    double sp_limited[NUM_MOTORS];
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      if(sp[i]>0) target_degrees[i]=180.0; // Must correct for JL
      else target_degrees[i]=-180.0;
      if (sp[i]>100) sp_limited[i]=100;
      else if (sp[i]<-100) sp_limited[i]=-100;
      else sp_limited[i]=sp[i];
      joint_vel[i] = (THREAD_RATE*SPEED_ADJ_V*sp_limited[i])/(100.0);
      printf("Vel %d: %f",i,joint_vel[i]);
      joint_status[i]=3;
    }
    printf("\n");
    return true;
}

