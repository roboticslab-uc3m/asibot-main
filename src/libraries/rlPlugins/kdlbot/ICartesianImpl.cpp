
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// -----------------------------------------------------------------------------

bool KdlBot::setTrackingMode(const bool f) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getTrackingMode(bool *f) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o) {  // interface is in [deg]
    double dRealDeg[numMotors];
    if(!enc->getEncoders(dRealDeg)) {
        printf("[warning] KdlBot::getPose() failed to getEncoders()\n");
        return false;
    }
    yarp::sig::Vector realDeg(numMotors,dRealDeg);
    return fwdKin(realDeg,x,o);
}

// ----------------------------------------------------------------------------- 

bool KdlBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t) {
    printf("KdlBot::goToPose() Begin setting absolute base movement.\n");
    targetX[0]=xd[0];
    targetX[1]=xd[1];
    targetX[2]=xd[2];
    targetO[0]=od[0];
    targetO[1]=od[1];
//    printf("KdlBot::goToPose() Problem statement:\n");
//    printf("xd: %s\nod: %s\n",xd.toString().c_str(),od.toString().c_str());
    yarp::sig::Vector x,o;
    getPose(x,o);
    double trajT=duration;
    if (t>0) trajT = t;
/*    printf("[goToPose] begin: trajPrP dump(100 samples).\n");
    trajPrP->dump(100);
    printf("[goToPose] end: trajPrP dump(100 samples).\n");
    printf("[goToPose] begin: trajPhP dump(100 samples).\n");
    trajPhP->dump(100);
    printf("[goToPose] end: trajPhP dump(100 samples).\n");
    printf("[goToPose] begin: trajOyP dump(100 samples).\n");
    trajOyP->dump(100);
    printf("[goToPose] end: trajOyP dump(100 samples).\n");
    printf("[goToPose] begin: trajOz dump(100 samples).\n");
    trajOz->dump(100);
    printf("[goToPose] end: trajOz dump(100 samples).\n");
    printf("[goToPose] begin: trajOzPP dump(100 samples).\n");
    trajOzPP->dump(100);
    printf("[goToPose] end: trajOzPP dump(100 samples).\n");*/
    startTime = Time::now();
    withOri=true;
    vel->setVelocityMode();
    cmc_status=1;
    printf("KdlBot::goToPose() End setting absolute base movement.\n");
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPosition(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                              const double t) {
    printf("[KdlBot] Begin setting absolute base movement.\n");
    targetX[0]=xd[0];
    targetX[1]=xd[1];
    targetX[2]=xd[2];
    targetO[0]=od[0];
    targetO[1]=od[1];
    yarp::sig::Vector x,o;
    getPose(x,o);
    double trajT=duration;
    if (t>0) trajT = t;
    printf("[goToPose] end: trajOzPP dump(100 samples).\n");
    startTime = Time::now();
    withOri=true;
    vel->setVelocityMode();
    cmc_status=1;
    printf("[KdlBot] End setting absolute base movement.\n");
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPositionSync(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getDesired(yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::askForPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                            yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::askForPose(const yarp::sig::Vector &q0,
                            const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                            yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                            yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::askForPosition(const yarp::sig::Vector &xd,
                                yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::askForPosition(const yarp::sig::Vector &q0,
                                const yarp::sig::Vector &xd,
                                yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                                yarp::sig::Vector &qdhat) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getDOF(yarp::sig::Vector &curDof) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setDOF(const yarp::sig::Vector &newDof, yarp::sig::Vector &curDof) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getRestPos(yarp::sig::Vector &curRestPos) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setRestPos(const yarp::sig::Vector &newRestPos, yarp::sig::Vector &curRestPos) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getRestWeights(yarp::sig::Vector &curRestWeights) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setRestWeights(const yarp::sig::Vector &newRestWeights, yarp::sig::Vector &curRestWeights) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getLimits(const int axis, double *min, double *max) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setLimits(const int axis, const double min, const double max) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getTrajTime(double *t) {
    *t = duration;
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::setTrajTime(const double t) {
    duration = t;
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::getInTargetTol(double *tol) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setInTargetTol(const double tol) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getJointsVelocities(yarp::sig::Vector &qdot) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getTaskVelocities(yarp::sig::Vector &xdot, yarp::sig::Vector &odot) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setTaskVelocities(const yarp::sig::Vector &xdot, const yarp::sig::Vector &odot) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::checkMotionDone(bool *f) {
    bool tmpf = false;
    if(cmc_status<=0) tmpf = true;
    *f = tmpf;
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::waitMotionDone(const double period, const double timeout) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::stopControl() {
    cmc_status=-1;
    printf("[end] KdlBot::stopControl()\n");
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::storeContext(int *id) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::restoreContext(const int id) {
    return false;
}

// -----------------------------------------------------------------------------

