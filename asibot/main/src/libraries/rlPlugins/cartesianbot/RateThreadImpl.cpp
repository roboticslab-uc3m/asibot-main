
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// -----------------------------------------------------------------------------

    /* Returns true if all have reached */
    bool CartesianBot::targetReached() {
        if ((targets[0])&&(fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0])))>CARTPOS_PRECISION) return false;
        if ((targets[1])&&(fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1])))>CARTPOS_PRECISION) return false;
        if ((targets[2])&&(fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2])))>CARTPOS_PRECISION) return false;
        double target_rpy[3] = {0,0,0};
        double real_rpy[3] = {0,0,0};
        real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
        target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
        if ((targets[3])&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0])))>CARTORI_PRECISION) return false;
        if ((targets[4])&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1])))>CARTORI_PRECISION) return false;
        if ((targets[5])&&(fabs(toDeg(target_rpy[2])-toDeg(real_rpy[2])))>CARTORI_PRECISION) return false;
        return true;
    }

    double CartesianBot::toDeg(const double inRad) {
        return (inRad * 180.0 / 3.14159265);
    }

    double CartesianBot::toRad(const double inDeg) {
        return (inDeg * 3.14159265 / 180.0);
    }

// ------------------- RateThread Related ------------------------------------

bool CartesianBot::threadInit() {
    printf("threadInit(): ok\n");
    return true;
}

// -----------------------------------------------------------------------------

void CartesianBot::run() {
    if (cmc_status>0) {  // If it is movement
        double grabValues[NUM_MOTORS];
        if(!enc->getEncoders(grabValues))
            printf("GIGANTIC encoder WARNING\n");
        for (int i=0; i<NUM_MOTORS; i++)
            real_rad(i)=toRad(grabValues[i]);
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
        fksolver.JntToCart(real_rad,real_cartpos);
        if (targetReached()) {
            printf("Target reached\n");
            currentTime = 0;
            pos->setPositionMode();
            double real_rot[3];
            real_cartpos.M.GetRPY(real_rot[0],real_rot[1],real_rot[2]);
            cmc_status=0;
        } else {
            //printf("Inside control loop moving.\n");
            //   Twist axis_vel;
            Twist T_desired = currentTrajectory->Vel(currentTime);
            Frame F_desired = currentTrajectory->Pos(currentTime);
            Twist T_current = diff(real_cartpos, F_desired);
            for (unsigned int i=0; i<6; i++) {
                T_current(i) *= GAIN;
                T_current(i) += T_desired(i);
                //T_current(i) += T_other(i);
            }
            JntArray joint_vel(5);  // in radians
            ChainIkSolverVel_pinv iksolverv_pinv(theChain);
            //int return_code = iksolverv_pinv.CartToJnt(real_rad,axis_vel,joint_vel);
            //int return_code = iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
            iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
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
                printf("GIGANTIC velocity WARNING\n");
            currentTime = currentTime + TIMEINCREMENT;
        }
    } else {  // If it is stopped or breaked, reamain unchanged
//j//        printf("Inside control loop stopped.\n");
    }
}

// -----------------------------------------------------------------------------

