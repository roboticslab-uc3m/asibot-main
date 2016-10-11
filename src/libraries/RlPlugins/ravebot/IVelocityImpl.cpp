// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

// ------------------ IVelocity Related ----------------------------------------

bool RaveBot::velocityMove(int j, double sp) {  // velExposed = sp;
    if ((unsigned int)j>numMotors) return false;
    if(modePosVel!=1) {  // Check if we are in velocity mode.
        fprintf(stderr,"[RaveBot] fail: RaveBot will not velocityMove as not in velocityMode\n");
        return false;
    }
    velRaw[j] = (sp * velRawExposed[j]);
    jointStatus[j]=3;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(const double *sp) {
    printf("[RaveBot] Vel:");
    for (unsigned int i=0; i<numMotors; i++) printf(" %+.6f",velRaw[i]);
    printf("\n");
    bool ok = true;
    for(unsigned int i=0;i<numMotors;i++)
        ok &= velocityMove(i,sp[i]);
    return ok;
}

// ----------------------------------------------------------------------------

