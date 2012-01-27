
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
    double grabValues[NUM_MOTORS];
    if(!enc->getEncoders(grabValues)) {
        printf("[warning] CartesianBot::getPose() failed to getEncoders()\n");
        return false;
    }
    for (int i=0; i<NUM_MOTORS; i++)
        realRad(i)=toRad(grabValues[i]);
    double pxP = A1*sin(realRad(1))+A2*sin(realRad(1)+realRad(2))+A3*sin(realRad(1)+realRad(2)+realRad(3)); // P = prime
    double pzP = A0+A1*cos(realRad(1))+A2*cos(realRad(1)+realRad(2))+A3*cos(realRad(1)+realRad(2)+realRad(3));
    double oyP = grabValues[1] + grabValues[2] + grabValues[3];  // [deg]
    x.resize(3);
    x[0] = pxP*cos(realRad(0));
    x[1] = pxP*sin(realRad(0));
    x[2] = pzP;  // pz = pzP
    o.resize(2);
    o[0] = oyP; // = pitchP
    o[1] = grabValues[4];  // = rollP
    return true;
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
    trajoz.configure(toDeg(atan2(x[1],x[0])),toDeg(atan2(xd[1],xd[0])),trajT);
    trajXP.configure(sqrt(pow(x[0],2)+pow(x[1],2)),sqrt(pow(xd[0],2)+pow(xd[1],2)),trajT);
    trajZP.configure(x[2]-A0,xd[2]-A0,trajT);
    trajPitchP.configure(o[0],od[0],trajT);  // We set it in degrees
    trajRollP.configure(o[1],od[1],trajT);  // We set it in degrees
    startTime = Time::now();
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
    double odzRad = atan2(xd[1],xd[0]);
    double xdP = sqrt(xd[0]*xd[0]+xd[1]*xd[1]);
    double zdP = xd[2]-A0;
    double oydP = od[0];
    printf("Problem statement:\n");
    printf("odz: %f\nxdP: %f\nzdP: %f\n",toDeg(odzRad),xdP,zdP);
    // t1=qdhat[1],t1=qdhat[2],t1=qdhat[3]
    double pWx = xdP - A3*sin(toRad(oydP));
    double pWz = zdP - A3*cos(toRad(oydP));
    double ct2 = (pow(pWx,2) + pow(pWz,2) - A1*A1 - A2*A2)/(2*A1*A2);
    double st2 = sqrt(1-ct2);  // forces elbow-up in ASIBOT
    //double st2 = -sqrt(1-ct2);  // forces elbow-down in ASIBOT
    double t2Rad = atan2(st2,ct2);
    double st1 = ((A1+A2*ct2)*pWx - A2*st2*pWz)/(pow(pWz,2)+pow(pWx,2));
    double ct1 = ((A1+A2*ct2)*pWz - A2*st2*pWx)/(pow(pWz,2)+pow(pWx,2));
    double t1Rad = atan2(st1,ct1);
    qdhat.resize(5);
    qdhat[0] = toDeg(odzRad);
    qdhat[1] = toDeg(t1Rad);
    qdhat[2] = toDeg(t2Rad);
    qdhat[3] = oydP - qdhat[1] - qdhat[2];
    qdhat[4] = od[1];
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
    if(cmc_status==0) tmpf = true;
/*    if (fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0]))>CARTPOS_PRECISION) tmpf = false;
    if (fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1]))>CARTPOS_PRECISION) tmpf = false;
    if (fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2]))>CARTPOS_PRECISION) tmpf = false;
    double target_rpy[3] = {0,0,0};
    double real_rpy[3] = {0,0,0};
    real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
    target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
    if ((withOri)&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0]))>CARTORI_PRECISION)) tmpf = false;
    if ((withOri)&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1]))>CARTORI_PRECISION)) tmpf = false;*/
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

