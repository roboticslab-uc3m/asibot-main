
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

bool KdlBot::getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o, yarp::os::Stamp *stamp) {
    return false;
}

// ----------------------------------------------------------------------------- 

bool KdlBot::getPose(yarp::sig::Vector &x, yarp::sig::Vector &o, yarp::os::Stamp *stamp) {
    double dRealUnits[100];
    if(!enc->getEncoders(dRealUnits)) {
        printf("[warning] KdlBot::getPose() failed to getEncoders()\n");
        return false;
    }
    yarp::sig::Vector realUnits(cmcNumMotors,dRealUnits);
    return fwdKin(realUnits,x,o);
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPosition(const yarp::sig::Vector &xd, const double t) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                              const double t) {
    printf("[KdlBot] Begin setting absolute base movement.\n");

    for (int s=0;s<od.size();s++)
        targetO[s]=od[s];

    targetF.p.data[0] = xd[0];
    targetF.p.data[1] = xd[1];
    targetF.p.data[2] = xd[2];
    if (angleRepr == "eulerYZ") {  // ASIBOT
        targetF.M = Rotation::EulerZYZ(::atan2(xd[1],xd[0]),toRad(od[0]), toRad(od[1]));
    } else if (angleRepr == "eulerZYZ") {
        targetF.M = Rotation::EulerZYZ(toRad(od[0]), toRad(od[1]), toRad(od[2]));
    } else {  // axisAngle, etc.
        printf("[warning] KDL no compatible angleRepr\n");
    }

    yarp::sig::Vector x,o;
    if(!getPose(x,o)) {
        printf("[goToPoseSync] getPose failed.\n");
        return false;
    }
    KDL::Frame initF;
    initF.p.data[0] = x[0];
    initF.p.data[1] = x[1];
    initF.p.data[2] = x[2];
    if (angleRepr == "eulerYZ") {  // ASIBOT
        initF.M = Rotation::EulerZYZ(::atan2(x[1],x[0]),toRad(o[0]), toRad(o[1]));
    } else if (angleRepr == "eulerZYZ") {
        initF.M = Rotation::EulerZYZ(toRad(o[0]), toRad(o[1]), toRad(o[2]));
    } else {  // axisAngle, etc.
        printf("[warning] KDL no compatible angleRepr\n");
    }

    double trajT=duration;
    if (t>0) trajT = t;

    _orient = new RotationalInterpolation_SingleAxis();
    double _eqradius = 1; //0.000001;
    bool _aggregate = true;
 
    KDL::Path_Line testPathLine(initF, targetF, _orient, _eqradius, _aggregate);
    KDL::VelocityProfile_Trap testVelocityProfile(maxVel, maxAcc);
//    KDL::Trajectory_Segment testTrajectory(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);
    currentTrajectory = new Trajectory_Segment(testPathLine.Clone(), testVelocityProfile.Clone(), duration, _aggregate);

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
    double currentUnits[100];
    if(!enc->getEncoders(currentUnits)) {
        printf("[warning] KdlBot::getPose() failed to getEncoders()\n");
        return false;
    }
    JntArray currentRads = JntArray(cmcNumMotors);
    for (int motor=0; motor<cmcNumMotors; motor++) {
        if(isPrismatic[motor]) currentRads(motor)=currentUnits[motor];
        else currentRads(motor)=toRad(currentUnits[motor]);
    }

    Frame frameXd;
    frameXd.p.data[0]=xd[0];
    frameXd.p.data[1]=xd[1];
    frameXd.p.data[2]=xd[2];
    if (angleRepr == "eulerYZ") {
        frameXd.M = Rotation::EulerZYZ(::atan2(xd[1],xd[0]),toRad(od[0]), toRad(od[1]));
    } else if (angleRepr == "eulerZYZ") {
        frameXd.M = Rotation::EulerZYZ(toRad(od[0]), toRad(od[1]), toRad(od[2]));
    } else {
        printf("[warning] KDL no compatible angleRepr\n");
    }

    ChainFkSolverPos_recursive fksolver(theChain);
    ChainIkSolverVel_pinv iksolver_vel(theChain);
//    ChainIkSolverPos_NR iksolver_pos (theChain,fksolver,iksolver_vel,500,1e-6);
    JntArray qMin = JntArray(cmcNumMotors);
    JntArray qMax = JntArray(cmcNumMotors);
    for (int motor=0; motor<cmcNumMotors; motor++) {
        double _qMin, _qMax;
        if(!lim->getLimits(motor,&(_qMin),&(_qMax))) return false;
        if(isPrismatic[motor]) {
            qMin(motor)=_qMin;
            qMax(motor)=_qMax;
        } else {
            qMin(motor)=toRad(_qMin);
            qMax(motor)=toRad(_qMax);
        }
    }
    ChainIkSolverPos_NR_JL iksolver_pos (theChain,qMin,qMax,fksolver,iksolver_vel,500,1e-6);

    JntArray qd = JntArray(cmcNumMotors);
    int ret = iksolver_pos.CartToJnt(currentRads,frameXd,qd);
    printf("[HelperFuncs] KDL ret = %d:\n",ret);

    if(ret<0) return false;

    for (int motor=0; motor<cmcNumMotors; motor++) {
        if(isPrismatic[motor]) qdhat.push_back(qd(motor));
        else qdhat.push_back(toDeg(qd(motor)));
    }

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
    if(!lim->getLimits(axis,min,max)) return false;  // should pass to JMC
    printf("Range of axis %d is: %f to %f.\n",axis,*min,*max);
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setLimits(const int axis, const double min, const double max) {
    if(!lim->setLimits(axis,min,max)) return false;  // should pass to JMC
    printf("[KdlBotRange of axis %d set to: %f to %f on JMC\n",axis,min,max);
    return true;
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

bool KdlBot::attachTipFrame(const yarp::sig::Vector &x, const yarp::sig::Vector &o) {
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::getTipFrame(yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::removeTipFrame() {
    return true;
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

bool KdlBot::getInfo(yarp::os::Bottle &info) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::registerEvent(yarp::dev::CartesianEvent &event) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::unregisterEvent(yarp::dev::CartesianEvent &event) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::setReferenceMode(const bool f) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::getReferenceMode(bool *f) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::tweakSet(const yarp::os::Bottle &options) {
    printf("[KdlBot] tweakSet: %s\n", options.toString().c_str());
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::tweakGet(yarp::os::Bottle &options) {
    return false;
}

// -----------------------------------------------------------------------------

bool KdlBot::deleteContext(const int id) {
    return false;
}

// -----------------------------------------------------------------------------

