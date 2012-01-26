
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- RateThread Related ------------------------------------

bool CartesianBot::threadInit() {
    printf("[success] threadInit(): Started %f ms ratethread\n",getRate());
    return true;
}

// -----------------------------------------------------------------------------

void CartesianBot::run() {
    if (cmc_status>0) {  // If it is movement
        yarp::sig::Vector x,o;
        getPose(x,o);
        bool done = false;
        checkMotionDone(&done);
        if (done) {
            printf("Target reached\n");
            startTime = 0;
            pos->setPositionMode();
            cmc_status=0;
        }/* else {
            //printf("Inside control loop moving.\n");
            Twist T_desired = currentTrajectory->Vel(currentTime);
            Frame F_desired = currentTrajectory->Pos(currentTime);
            Twist T_current = diff(real_cartpos, F_desired);
            for (unsigned int i=0; i<6; i++) {
                T_current(i) *= GAIN;
                T_current(i) += T_desired(i);
            }
            JntArray joint_vel(5);  // in radians
//            ChainIkSolverVel_pinv iksolverv_pinv(theChain);
//            iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
            double cmc_qdot[5];
            for (unsigned int i=0; i<5; i++)
                cmc_qdot[i]=toDeg(joint_vel(i));
            // Should normalize anyway (Find out the max, etc)
            double max_qdot = 100;
            for (int motor=0; motor<5; motor++)
                if (fabs(cmc_qdot[motor])>max_qdot)
                    max_qdot = fabs(cmc_qdot[motor]);
            if (max_qdot > 100)
                for (int motor=0; motor<5; motor++)
                    cmc_qdot[motor]=(cmc_qdot[motor])*100.0/max_qdot;
            if(!vel->velocityMove(cmc_qdot))
                printf("[warning] CartesianBot::run() failed to velocityMove()\n");
            currentTime = currentTime + TIMEINCREMENT;
        }*/
    } else {  // If it is stopped or breaked, reamain unchanged
//j//        printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

