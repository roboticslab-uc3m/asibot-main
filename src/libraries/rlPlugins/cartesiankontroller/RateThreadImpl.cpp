
// ----------------------------------------------------------------------------

#include "CartesianKontroller.h"

// ----------------------------------------------------------------------------


bool CartesianKontroller::threadInit() {
    // 5 DOF chain for FK solver
    // Note that does not include length of first link.
    theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.12))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.12))));
    theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.16))));
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
    real_rad = JntArray(5);
    fksolver.JntToCart(real_rad,real_cartpos);
    fksolver.JntToCart(real_rad,target_cartpos);
    vgeneral = 100;
    cmc_status = 0;
    _orient = new RotationalInterpolation_SingleAxis();
    _eqradius = 1; //0.000001;
    _aggregate = false;
    currentTime = 0;
    seekOri = false;

    return true;
}


void CartesianKontroller::run() {
    if (cmc_status>0) {  // If it is movement
        double encs[MAX_NUM_MOTORS];
        enc->getEncoders(encs);
        for (int motor=0; motor<num_motors; motor++)
            real_rad(motor)=toRad(encs[motor]);
        ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
        ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
        fksolver.JntToCart(real_rad,real_cartpos);
        if (targetReached(seekOri)) { //depends
            printf("Target reached \n");
            if (seekOri) printf("(pos+ori).\n");
            else printf("(pos).\n");
            currentTime = 0;
            vel->stop(); // Set vel of all motors to 0
            // Publish state if reached?
            cmc_status=0;
        } else {
            //printf("Inside control loop moving.\n");
            Twist axis_vel;
            Twist T_desired = currentTrajectory->Vel(currentTime);
            Frame F_desired = currentTrajectory->Pos(currentTime);
            Twist T_current = diff(real_cartpos, F_desired);

            for (unsigned int i=0; i<6; i++) {
                T_current(i) *= GAIN;
                T_current(i) += T_desired(i);  // ... T_current(i) += T_other(i);
            }

            JntArray joint_vel(num_motors);  // in radians
            ChainIkSolverVel_pinv iksolverv_pinv(theChain);
            iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
            double cmc_qdot[MAX_NUM_MOTORS];
            for (unsigned int i=0; i<num_motors; i++)
                cmc_qdot[i]=toDeg(joint_vel(i));
            // Should normalize anyway (Find out the max, etc)
            double max_qdot = 100;
            for (int motor=0; motor<num_motors; motor++)
                if (fabs(cmc_qdot[motor])>max_qdot)
                    max_qdot = fabs(cmc_qdot[motor]);
            if (max_qdot > 100)
                for (int motor=0; motor<num_motors; motor++)
                    cmc_qdot[motor]=(cmc_qdot[motor])*100.0/max_qdot;
            vel->velocityMove(cmc_qdot);
            currentTime = currentTime + TIMEINCREMENT;
        }
    } else {  // If it is stopped or breaked, reamain unchanged
        //printf("Inside control loop stopped.\n");
    }
}


bool CartesianKontroller::targetReached(bool withOri) {
    if (fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0]))>CARTPOS_PRECISION) return false;
    if (fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1]))>CARTPOS_PRECISION) return false;
    if (fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2]))>CARTPOS_PRECISION) return false;
    double target_rpy[3] = {0,0,0};
    double real_rpy[3] = {0,0,0};
    real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
    target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
    if ((withOri)&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0])))>CARTORI_PRECISION) return false;
    if ((withOri)&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1])))>CARTORI_PRECISION) return false;
    if ((withOri)&&(fabs(toDeg(target_rpy[2])-toDeg(real_rpy[2])))>CARTORI_PRECISION) return false;
    return true;
}

