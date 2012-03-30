
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
    if(modePosVel!=1) {
        printf("RaveBot: Not in velocity mode.\n");
        return false;
    }
    if(sp>0) targetDeg[j]=180.0; // Must correct for JL
    else targetDeg[j]=-180.0;
    // Let's not limit it for now:
//    if (sp>100) sp=100;
//    else if (sp<-100) sp=-100;
    // Force it as is:
    jointVel[j] = sp;
    //jointVel[j] = (THREAD_RATE*SPEED_ADJ_V*sp)/(100.0);
    joint_status[j]=3;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(const double *sp) {
    if(modePosVel!=1) {
        printf("RaveBot: Not in velocity mode.\n");
        return false;
    }
//    double sp_limited[numMotors];
    printf("Vel:");
    for (unsigned int i=0; i<numMotors; i++) {
        if(sp[i]>0) targetDeg[i]=180.0; // Must correct for JL
        else targetDeg[i]=-180.0;
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

