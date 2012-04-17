
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IVelocity Related ----------------------------------------

bool RaveBot::setVelocityMode() {
//    printf("RaveBot::setVelocityMode()\n");
    if (modePosVel==1) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    modePosVel = 1;  // Set flag to vel.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(int j, double sp) {
    if(modePosVel!=1) {  // Check if we are in velocity mode.
        if(!setVelocityMode()) return false;  // Don't do anything if we can't set velocity mode.
    }
    if(sp>0) targetDeg[j]=maxLimit[j];
    else targetDeg[j]=minLimit[j];
    // Let's not limit it for now:
//    if (sp>100) sp=100;
//    else if (sp<-100) sp=-100;
    // Force it as is:
    jointVel[j] = sp;
    joint_status[j]=3;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(const double *sp) {
    if(modePosVel!=1) {  // Check if we are in velocity mode.
        if(!setVelocityMode()) return false;  // Don't do anything if we can't set velocity mode.
    }
    // Let's not limit it for now:
//    double sp_limited[numMotors];
    printf("Vel:");
    for (unsigned int i=0; i<numMotors; i++) {
        if(sp[i]>0) targetDeg[i]=maxLimit[i];
        else targetDeg[i]=minLimit[i];
//      if (sp[i]>100) sp_limited[i]=100;
//      else if (sp[i]<-100) sp_limited[i]=-100;
//      else sp_limited[i]=sp[i];
//      jointVel[i] = (THREAD_RATE*SPEED_ADJ_V*sp_limited[i])/(100.0);
        // Force it as is:
        jointVel[i] = sp[i];
        printf(" %+.6f",jointVel[i]);
        joint_status[i]=3;
    }
    printf("\n");
    return true;
}

