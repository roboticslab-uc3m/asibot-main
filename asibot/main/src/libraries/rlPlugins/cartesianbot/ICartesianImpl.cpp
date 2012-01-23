
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// -----------------------------------------------------------------------------

bool CartesianBot::setTrackingMode(const bool f) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getTrackingMode(bool *f) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o) {
    double grabValues[NUM_MOTORS];
    if(!enc->getEncoders(grabValues)) {
        printf("[warning] CartesianBot::getPose() failed to getEncoders()\n");
        return false;
    }
    for (int i=0; i<NUM_MOTORS; i++)
        realRad(i)=toRad(grabValues[i]);
    double pxP = A1*cos(realRad(1))+A2*cos(realRad(1)+realRad(2))+A3*cos(realRad(1)+realRad(2)+realRad(3));
    double pzP = A1*sin(realRad(1))+A2*sin(realRad(1)+realRad(2))+A3*sin(realRad(1)+realRad(2)+realRad(3));
    double oyP = grabValues[1] + grabValues[2] + grabValues[3];
//    pFksolver->JntToCart(real_rad,real_cartpos);
//    KDL::Vector axis = real_cartpos.M.GetRot();  // Gives only rotation axis
//    double angle = real_cartpos.M.GetRotAngle(axis);  // Gives both angle and rotation axis
/*    x.push_back(real_cartpos.p.data[0]);
    x.push_back(real_cartpos.p.data[1]);
    x.push_back(real_cartpos.p.data[2]);
    o.push_back(axis[0]);
    o.push_back(axis[1]);
    o.push_back(axis[2]);
    o.push_back(angle);*/
    return true;
}

// ----------------------------------------------------------------------------- 

bool CartesianBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t) {
    printf("[cmc_kdl] Begin absolute base movement.\n");
    double grabValues[NUM_MOTORS];
    if(!enc->getEncoders(grabValues)) {
        printf("[warning] CartesianBot::goToPose() failed to getEncoders()\n");
        return false;
    }
    for (int i=0; i<NUM_MOTORS; i++)
        realRad(i)=toRad(grabValues[i]);
    vel->setVelocityMode();
//    pFksolver->JntToCart(real_rad,real_cartpos);

/*    target_cartpos.p.x(xd[0]);
    target_cartpos.p.y(xd[1]);
    target_cartpos.p.z(xd[2]);
    KDL::Vector rotvec(od[0],od[1],od[2]);
    target_cartpos.M = Rotation::Rot(rotvec,od[4]);

    KDL::Path_Line testPathLine(real_cartpos, target_cartpos, _orient, _eqradius, _aggregate);
    //KDL::VelocityProfile_Rectangular::VelocityProfile_Rectangular(double _maxvel = 0)
    double _maxVel = 0.2; //0.1; //?
    double  maxAcc = 0.2; //0.1; //?
    //KDL::VelocityProfile_Rectangular testVelocityProfile(_maxVel);
    KDL::VelocityProfile_Trap testVelocityProfile(_maxVel, maxAcc);
    //Trajectory_Segment (Path *_geom, VelocityProfile *_motprof, double duration, bool _aggregate=true)
    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);
    //currentTrajectory = KDL::Trajectory_Segment(testPathLine.Clone(), testVelocityProfile.Clone(), DURATION, _aggregate);
    //delete currentTrajectory;
    currentTrajectory = testTrajectory.Clone();
    // Set the status
    currentTime = 0;
*/
    // Set the status

    cmc_status=1;

    printf("[cmc_kdl] End absolute base movement.\n");
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPosition(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                              const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPositionSync(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getDesired(yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::askForPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                            yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::askForPose(const yarp::sig::Vector &q0,
                            const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                            yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::askForPosition(const yarp::sig::Vector &xd,
                                yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::askForPosition(const yarp::sig::Vector &q0,
                                const yarp::sig::Vector &xd,
                                yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getDOF(yarp::sig::Vector &curDof) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setDOF(const yarp::sig::Vector &newDof, yarp::sig::Vector &curDof) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getRestPos(yarp::sig::Vector &curRestPos) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setRestPos(const yarp::sig::Vector &newRestPos, yarp::sig::Vector &curRestPos) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getRestWeights(yarp::sig::Vector &curRestWeights) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setRestWeights(const yarp::sig::Vector &newRestWeights, yarp::sig::Vector &curRestWeights) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getLimits(const int axis, double *min, double *max) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setLimits(const int axis, const double min, const double max) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getTrajTime(double *t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setTrajTime(const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getInTargetTol(double *tol) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setInTargetTol(const double tol) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getJointsVelocities(yarp::sig::Vector &qdot) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getTaskVelocities(yarp::sig::Vector &xdot, yarp::sig::Vector &odot) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setTaskVelocities(const yarp::sig::Vector &xdot, const yarp::sig::Vector &odot) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::checkMotionDone(bool *f) {
    bool tmpf = true;
    if (fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0]))>CARTPOS_PRECISION) tmpf = false;
    if (fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1]))>CARTPOS_PRECISION) tmpf = false;
    if (fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2]))>CARTPOS_PRECISION) tmpf = false;
    double target_rpy[3] = {0,0,0};
    double real_rpy[3] = {0,0,0};
    real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
    target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
    if ((withOri)&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0]))>CARTORI_PRECISION)) tmpf = false;
    if ((withOri)&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1]))>CARTORI_PRECISION)) tmpf = false;
    if ((withOri)&&(fabs(toDeg(target_rpy[2])-toDeg(real_rpy[2]))>CARTORI_PRECISION)) tmpf = false;
    *f = tmpf;
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::waitMotionDone(const double period, const double timeout) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::stopControl() {
    cmc_status=-1;
    printf("[end] CartesianBot::stopControl()\n");
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::storeContext(int *id) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::restoreContext(const int id) {
    return false;
}

// -----------------------------------------------------------------------------

