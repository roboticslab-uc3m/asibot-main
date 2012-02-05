
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

bool CartesianBot::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o) {  // interface is in [deg]
    double realDeg[NUM_MOTORS];
    if(!enc->getEncoders(realDeg)) {
        printf("[warning] CartesianBot::getPose() failed to getEncoders()\n");
        return false;
    }
    return fwdKin(realDeg,x,o);
}

// ----------------------------------------------------------------------------- 

bool CartesianBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t) {
    printf("[CartesianBot] Begin setting absolute base movement.\n");
    targetX[0]=xd[0];
    targetX[1]=xd[1];
    targetX[2]=xd[2];
    targetO[0]=od[0];
    targetO[1]=od[1];
    yarp::sig::Vector x,o;
    getPose(x,o);
    double trajT=duration;
    if (t>0) trajT = t;
    trajPrP.configure(sqrt((x[0])*(x[0])+(x[1])*(x[1])),sqrt((xd[0])*(xd[0]))+((xd[1])*(xd[1])),trajT);
    trajPhP.configure(x[2]-A0,xd[2]-A0,trajT);
    trajOyP.configure(o[0],od[0],trajT);  // We set it in degrees
    trajOz.configure(toDeg(atan2(x[1],x[0])),toDeg(atan2(xd[1],xd[0])),trajT);
    trajOzPP.configure(o[1],od[1],trajT);  // We set it in degrees
    printf("[goToPose] begin: trajPrP dump(100 samples).\n");
    trajPrP.dump(100);
    printf("[goToPose] end: trajPrP dump(100 samples).\n");
    printf("[goToPose] begin: trajPhP dump(100 samples).\n");
    trajPhP.dump(100);
    printf("[goToPose] end: trajPhP dump(100 samples).\n");
    printf("[goToPose] begin: trajOyP dump(100 samples).\n");
    trajOyP.dump(100);
    printf("[goToPose] end: trajOyP dump(100 samples).\n");
    startTime = Time::now();
    withOri=true;
    vel->setVelocityMode();
    cmc_status=1;
    printf("[CartesianBot] End setting absolute base movement.\n");
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
    printf("Problem statement:\n");
    printf("xd: %s\nod: %s\n",xd.toString().c_str(),od.toString().c_str());
    double ozdRad = atan2(xd[1],xd[0]);
    double prPd = sqrt(xd[0]*xd[0]+xd[1]*xd[1]);
    double phPd = xd[2]-A0;
    double oyPd = od[0];
    printf("Problem statement:\n");
    printf("ozd: %f\nprPd: %f\nphPd: %f\n",toDeg(ozdRad),prPd,phPd);
    // t1=qdhat[1],t2=qdhat[2],t3=qdhat[3]
    double prWd = prPd - A3*sin(toRad(oyPd));
    double phWd = phPd - A3*cos(toRad(oyPd));
    double ct2 = (prWd*prWd + phWd*phWd - A1*A1 - A2*A2)/(2*A1*A2);
    double st2 = sqrt(1-ct2*ct2);  // forces elbow-up in ASIBOT
    //double st2 = -sqrt(1-ct2*ct2);  // forces elbow-down in ASIBOT
    printf("prWd: %f, phWd:%f\n",prWd,phWd);
    double t2Rad = atan2(st2,ct2);
    double st1 = ((A1+A2*ct2)*prWd - A2*st2*phWd)/(phWd*phWd+prWd*prWd);
    double ct1 = ((A1+A2*ct2)*phWd + A2*st2*prWd)/(phWd*phWd+prWd*prWd);
    // double ct1 = (phWd+A2*st1*st2)/(A1+A2*ct2);  // Alternative method for same result
    double t1Rad = atan2(st1,ct1);
    qdhat.resize(5);
    qdhat[0] = toDeg(ozdRad);
    qdhat[1] = toDeg(t1Rad);
    qdhat[2] = toDeg(t2Rad);
    qdhat[3] = oyPd - qdhat[1] - qdhat[2];
    qdhat[4] = od[1];  // ozPP
// Do the fwd kin for this and then:
    xdhat.resize(3);
//    xdhat[0] = ;
    odhat.resize(2);
//    odhat[0] = ;
    return true;
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
    *t = duration;
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setTrajTime(const double t) {
    duration = t;
    return true;
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
    bool tmpf = false;
    if(cmc_status<=0) tmpf = true;
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

