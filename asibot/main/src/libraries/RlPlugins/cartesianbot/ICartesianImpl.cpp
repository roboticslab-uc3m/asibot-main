
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

bool CartesianBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o, yarp::os::Stamp *stamp) {
    return false;
}

// ----------------------------------------------------------------------------- 

bool CartesianBot::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o, yarp::os::Stamp *stamp) {
    double realDeg[NUM_MOTORS];
    if(!enc->getEncoders(realDeg)) {
        printf("[warning] CartesianBot::getPose() failed to getEncoders()\n");
        return false;
    }
    return fwdKin(realDeg,x,o);
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t) {
    printf("CartesianBot::goToPose() Begin setting absolute base movement.\n");
    targetX[0]=xd[0];
    targetX[1]=xd[1];
    targetX[2]=xd[2];
    targetO[0]=od[0];
    targetO[1]=od[1];
//    printf("CartesianBot::goToPose() Problem statement:\n");
//    printf("xd: %s\nod: %s\n",xd.toString().c_str(),od.toString().c_str());
    yarp::sig::Vector x,o;
    getPose(x,o);
    double trajT=duration;
    if (t>0) trajT = t;
    trajPrP = new OrderOneTraj;
    trajPrP->configure(sqrt(x[0]*x[0]+x[1]*x[1]),sqrt(xd[0]*xd[0]+xd[1]*xd[1]),trajT);
    trajPhP = new OrderOneTraj;
    trajPhP->configure(x[2]-A0,xd[2]-A0,trajT);
    trajOyP = new OrderOneTraj;
    trajOyP->configure(o[0],od[0],trajT);  // We set it in degrees
    trajOz = new OrderOneTraj;
    trajOz->configure(toDeg(atan2(x[1],x[0])),toDeg(atan2(xd[1],xd[0])),trajT);
    trajOzPP = new OrderOneTraj;
    trajOzPP->configure(o[1],od[1],trajT);  // We set it in degrees
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
    printf("CartesianBot::goToPose() End setting absolute base movement.\n");
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPosition(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                              const double t) {
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
    trajPrP = new OrderThreeTraj;
    trajPrP->configure(sqrt(x[0]*x[0]+x[1]*x[1]),sqrt(xd[0]*xd[0]+xd[1]*xd[1]),trajT);
    trajPhP = new OrderThreeTraj;
    trajPhP->configure(x[2]-A0,xd[2]-A0,trajT);
    trajOyP = new OrderThreeTraj;
    trajOyP->configure(o[0],od[0],trajT);  // We set it in degrees
    trajOz = new OrderThreeTraj;
    trajOz->configure(toDeg(atan2(x[1],x[0])),toDeg(atan2(xd[1],xd[0])),trajT);
    trajOzPP = new OrderThreeTraj;
    trajOzPP->configure(o[1],od[1],trajT);  // We set it in degrees
    printf("[goToPose] begin: trajPrP dump(100 samples).\n");
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
    printf("[goToPose] end: trajOzPP dump(100 samples).\n");
    startTime = Time::now();
    withOri=true;
    vel->setVelocityMode();
    cmc_status=1;
    printf("[CartesianBot] End setting absolute base movement.\n");
    return true;
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
    double realDeg[NUM_MOTORS];
    if(!enc->getEncoders(realDeg)) {
        printf("[warning] CartesianBot::setTaskVelocities() failed to getEncoders()\n");
        return false;  // bad practice??
    }
    yarp::sig::Matrix Ja(5,5);
    for (int i=0; i<NUM_MOTORS; i++)
        realRad[i]=toRad(realDeg[i]);
    Ja(0,0) = -sin(realRad[0])*(A2*sin(realRad[1] + realRad[2]) + A1*sin(realRad[1]) + A3*sin(realRad[1] + realRad[2] + realRad[3]));
    Ja(0,1) = cos(realRad[0])*(A2*cos(realRad[1] + realRad[2]) + A1*cos(realRad[1]) + A3*cos(realRad[1] + realRad[2] + realRad[3]));
    Ja(0,2) = cos(realRad[0])*(A2*cos(realRad[1] + realRad[2]) + A3*cos(realRad[1] + realRad[2] + realRad[3]));
    Ja(0,3) = A3*cos(realRad[1] + realRad[2] + realRad[3])*cos(realRad[0]);
    Ja(0,4) = 0;
    Ja(1,0) = cos(realRad[0])*(A2*sin(realRad[1] + realRad[2]) + A1*sin(realRad[1]) + A3*sin(realRad[1] + realRad[2] + realRad[3]));
    Ja(1,1) = sin(realRad[0])*(A2*cos(realRad[1] + realRad[2]) + A1*cos(realRad[1]) + A3*cos(realRad[1] + realRad[2] + realRad[3]));
    Ja(1,2) = sin(realRad[0])*(A2*cos(realRad[1] + realRad[2]) + A3*cos(realRad[1] + realRad[2] + realRad[3]));
    Ja(1,3) = A3*cos(realRad[1] + realRad[2] + realRad[3])*sin(realRad[0]);
    Ja(1,4) = 0;
    Ja(2,0) = 0;
    Ja(2,1) = - A2*sin(realRad[1] + realRad[2]) - A1*sin(realRad[1]) - A3*sin(realRad[1] + realRad[2] + realRad[3]);
    Ja(2,2) = - A2*sin(realRad[1] + realRad[2]) - A3*sin(realRad[1] + realRad[2] + realRad[3]);
    Ja(2,3) = -A3*sin(realRad[1] + realRad[2] + realRad[3]);
    Ja(2,4) = 0;
    Ja(3,0) = 0;
    Ja(3,1) = 1;
    Ja(3,2) = 1;
    Ja(3,3) = 1;
    Ja(3,4) = 0;
    Ja(4,0) = 0;
    Ja(4,1) = 0;
    Ja(4,2) = 0;
    Ja(4,3) = 0;
    Ja(4,4) = 1;
    yarp::sig::Matrix Ja_pinv(pinv(Ja,1.0e-2));
    yarp::sig::Vector xdotd(xdot);
    xdotd.push_back(toRad(odot[0]));
    xdotd.push_back(toRad(odot[1]));
    yarp::sig::Vector t;
    t.resize(5);
    t = Ja_pinv * xdotd;
    double qdot[NUM_MOTORS];
    qdot[0] = toDeg(t[0]);
    qdot[1] = toDeg(t[1]);
    qdot[2] = toDeg(t[2]);
    qdot[3] = toDeg(t[3]);
    qdot[4] = toDeg(t[4]);
    vel->setVelocityMode();
    if(!vel->velocityMove(qdot))
        printf("GIGANTIC velocity WARNING\n");
    return true;
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

bool CartesianBot::getInfo(yarp::os::Bottle &info) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::registerEvent(yarp::dev::CartesianEvent &event) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::unregisterEvent(yarp::dev::CartesianEvent &event) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::setReferenceMode(const bool f) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::getReferenceMode(bool *f) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::tweakSet(const yarp::os::Bottle &options) {
    printf("[CartesianBot] tweakSet: %s\n", options.toString().c_str());
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::tweakGet(yarp::os::Bottle &options) {
    return false;
}

// -----------------------------------------------------------------------------

bool CartesianBot::deleteContext(const int id) {
    return false;
}

// -----------------------------------------------------------------------------

