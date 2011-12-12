
// ----------------------------------------------------------------------------

#include "CartesianKontroller.h"

// ----------------------------------------------------------------------------


bool CartesianKontroller::setTrackingMode(const bool f) {
    return true;
}


bool CartesianKontroller::getTrackingMode(bool *f) {
    return true;
}


bool CartesianKontroller::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o) {
    double encs[MAX_NUM_MOTORS];
    enc->getEncoders(encs);
    for (int motor=0; motor<num_motors; motor++)
        real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);
    x.clear();
    x.push_back(real_cartpos.p.data[0]);  // Adds memory slot and data
    x.push_back(real_cartpos.p.data[1]);  // Adds memory slot and data
    x.push_back(real_cartpos.p.data[2]);  // Adds memory slot and data
    o.resize(4);
    KDL::Vector rotVector = real_cartpos.M.GetRot();
    o[3] = real_cartpos.M.GetRotAngle(rotVector);  // Normalizes as colateral effect
    o[0] = rotVector[0];
    o[1] = rotVector[1];
    o[2] = rotVector[2];
    return true;
}


bool CartesianKontroller::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return true;
}


bool CartesianKontroller::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                                   const double t) {
    double encs[MAX_NUM_MOTORS];
    enc->getEncoders(encs);
    for (int motor=0; motor<num_motors; motor++)
        real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);
    target_cartpos.p.x(xd[0]);
    target_cartpos.p.y(xd[1]);
    target_cartpos.p.z(xd[2]);
    printf("HERE\n");
    KDL::Vector dRotVector(od[0],od[1],od[2]);
    target_cartpos.M = KDL::Rotation::Rot2(dRotVector,od[3]);
    // vgeneral=velocity; // TODO: Take velocity into account
    KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
    double _maxVel = 0.2; //0.1; //TODO
    double  maxAcc = 0.2; //0.1; //TODO
    KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
    currentTrajectory = testTrajectory.Clone();

    seekOri = true;
    cmc_status = 1;

    return true;
}


bool CartesianKontroller::goToPosition(const yarp::sig::Vector &xd, const double t) {
    double encs[MAX_NUM_MOTORS];
    enc->getEncoders(encs);
    for (int motor=0; motor<num_motors; motor++)
        real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);
    target_cartpos.p.x(xd[0]);
    target_cartpos.p.y(xd[1]);
    target_cartpos.p.z(xd[2]);
    // target_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
    // printf("No orientation taken into account.\n");
    // vgeneral=velocity; // TODO: Take velocity into account
    KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
    double _maxVel = 0.2; //0.1; //TODO
    double  maxAcc = 0.2; //0.1; //TODO
    KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
    currentTrajectory = testTrajectory.Clone();

    seekOri = false;
    cmc_status = 1;
    return true;
}


bool CartesianKontroller::goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                                       const double t) {
    return true;
}


bool CartesianKontroller::goToPositionSync(const yarp::sig::Vector &xd, const double t) {
    goToPosition(xd, t);  // Same behaviour for now
    return true;
}


bool CartesianKontroller::getDesired(yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                     yarp::sig::Vector &qdhat) {
    return true;
}


bool CartesianKontroller::askForPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                                     yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                     yarp::sig::Vector &qdhat) {
    return true;
}


bool CartesianKontroller::askForPose(const yarp::sig::Vector &q0,
                                     const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                                     yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                     yarp::sig::Vector &qdhat) {
    return true;
}


bool CartesianKontroller::askForPosition(const yarp::sig::Vector &xd,
        yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
        yarp::sig::Vector &qdhat) {
    return true;
}


bool CartesianKontroller::askForPosition(const yarp::sig::Vector &q0,
        const yarp::sig::Vector &xd,
        yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
        yarp::sig::Vector &qdhat) {
    return true;
}


bool CartesianKontroller::getDOF(yarp::sig::Vector &curDof) {
    return true;
}


bool CartesianKontroller::setDOF(const yarp::sig::Vector &newDof, yarp::sig::Vector &curDof) {
    return true;
}


bool CartesianKontroller::getRestPos(yarp::sig::Vector &curRestPos) {
    return true;
}


bool CartesianKontroller::setRestPos(const yarp::sig::Vector &newRestPos, yarp::sig::Vector &curRestPos) {
    return true;
}


bool CartesianKontroller::getRestWeights(yarp::sig::Vector &curRestWeights) {
    return true;
}


bool CartesianKontroller::setRestWeights(const yarp::sig::Vector &newRestWeights, yarp::sig::Vector &curRestWeights) {
    return true;
}


bool CartesianKontroller::getLimits(const int axis, double *min, double *max) {
    return true;
}


bool CartesianKontroller::setLimits(const int axis, const double min, const double max) {
    return true;
}

bool CartesianKontroller::getTrajTime(double *t) {
    return true;
}


bool CartesianKontroller::setTrajTime(const double t) {
    return true;
}


bool CartesianKontroller::getInTargetTol(double *tol) {
    return true;
}


bool CartesianKontroller::setInTargetTol(const double tol) {
    return true;
}


bool CartesianKontroller::getJointsVelocities(yarp::sig::Vector &qdot) {
    return true;
}

bool CartesianKontroller::getTaskVelocities(yarp::sig::Vector &xdot, yarp::sig::Vector &odot) {
    return true;
}


bool CartesianKontroller::checkMotionDone(bool *f) {
    if (cmc_status==0) *f = true;
    else *f = false;
    return true;
}


bool CartesianKontroller::waitMotionDone(const double period, const double timeout) {
    static double thisTime = 0.0;
    thisTime = 0.0;
    if (timeout != 0.0) {
        while (cmc_status!=0) {
            Time::delay(period);
            thisTime += (period);
            if (thisTime >= timeout) break;
        };
    } else {
        printf("[warning] no time limitation for waitMotionDone\n");
        while (cmc_status!=0); // Better default to something??
    }
    return true;
}


bool CartesianKontroller::stopControl() {
    cmc_status=0;
//j//
    vel->stop(); // Set vel of all motors to 0
    return true;
}


bool CartesianKontroller::storeContext(int *id) {
    return true;
}


bool CartesianKontroller::restoreContext(const int id) {
    return true;
}


bool CartesianKontroller::setTaskVelocities(const yarp::sig::Vector&, const yarp::sig::Vector&) {
    return true;
}


bool CartesianKontroller::goToRelativePose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
        const double t) {
    printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\n");
    double encs[MAX_NUM_MOTORS];
    enc->getEncoders(encs);
    for (int motor=0; motor<num_motors; motor++)
        real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);
    Frame newtarget_cartpos = Frame::Identity();
    newtarget_cartpos.p.x(xd[0]);
    newtarget_cartpos.p.y(xd[1]);
    newtarget_cartpos.p.z(xd[2]);
//        newtarget_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
    KDL::Vector target_rot(od[0],od[1],od[2]);
    newtarget_cartpos.M = Rotation::Rot(target_rot,od[3]);
    target_cartpos=real_cartpos*newtarget_cartpos;
    // printf("No orientation taken into account.\n");
    // vgeneral=velocity; // TODO: Take velocity into account
    KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
    double _maxVel = 0.2; //0.1; //TODO
    double  maxAcc = 0.2; //0.1; //TODO
    KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
    currentTrajectory = testTrajectory.Clone();

    seekOri = true;
    cmc_status = 1;

    return true;
}


bool CartesianKontroller::goToRelativePosition(const yarp::sig::Vector &xd, const double t) {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    double encs[MAX_NUM_MOTORS];
    enc->getEncoders(encs);
    for (int motor=0; motor<num_motors; motor++)
        real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);
    Frame newtarget_cartpos = Frame::Identity();
    newtarget_cartpos.p.x(xd[0]);
    newtarget_cartpos.p.y(xd[1]);
    newtarget_cartpos.p.z(xd[2]);
    target_cartpos=real_cartpos*newtarget_cartpos;
//    target_cartpos=newtarget_cartpos*real_cartpos; // base orientation
    // newtarget_cartpos.M = Rotation::RPY(toRad(double_x_i[3]),toRad(double_x_i[4]),toRad(double_x_i[5]));
    // printf("No orientation taken into account.\n");
    // vgeneral=velocity; // TODO: Take velocity into account
    KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
    double _maxVel = 0.2; //0.1; //TODO
    double  maxAcc = 0.2; //0.1; //TODO
    KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
    currentTrajectory = testTrajectory.Clone();

    seekOri = false;
    cmc_status = 1;

    return true;
}


bool CartesianKontroller::goToRelativePoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
        const double t) {
    return goToRelativePose(xd,od,t);
}


bool CartesianKontroller::goToRelativePositionSync(const yarp::sig::Vector &xd, const double t) {
    return goToRelativePosition(xd,t);
}

/**********************************************************/

double CartesianKontroller::toDeg(const double inRad) {
    return (inRad * 180.0 / 3.14159265);
}


double CartesianKontroller::toRad(const double inDeg) {
    return (inDeg * 3.14159265 / 180.0);
}

