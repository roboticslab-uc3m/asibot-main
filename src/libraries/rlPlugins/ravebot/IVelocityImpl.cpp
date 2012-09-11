
// -----------------------------------------------------------------------------

#include "RaveBot.h"

// ------------------ IVelocity Related ----------------------------------------

bool RaveBot::setVelocityMode() {
    printf("RaveBot::setVelocityMode()\n");
    if (modePosVel==1) return true;  // Simply return true if we were already in vel mode.
    // Do anything additional before setting flag to vel...
    modePosVel = 1;  // Set flag to vel.
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(int j, double sp) {
    if(modePosVel!=1) {  // Check if we are in velocity mode.
        printf("[fail] RaveBot will not velocityMove as not in velocityMode\n");
        return false;
    }
    if (j>=numMotors) return false;
    velRaw[j] = (sp * velRawExposed[j]);
    jointStatus[j]=3;
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::velocityMove(const double *sp) {
    printf("Vel:");
    for (unsigned int i=0; i<numMotors; i++) printf(" %+.6f",velRaw[i]);
    printf("\n");
    bool ok = true;
    for(int i=0;i<numMotors;i++)
        ok &= velocityMove(i,sp[i]);
    return ok;
}

// ----------------------------------------------------------------------------

