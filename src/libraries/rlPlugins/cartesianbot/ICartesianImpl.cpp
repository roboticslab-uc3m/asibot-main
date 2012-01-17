
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
    return false;
}

// ----------------------------------------------------------------------------- 

bool CartesianBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                          const double t) {
    return false;
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
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::waitMotionDone(const double period, const double timeout) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::stopControl() {
    return false;
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

