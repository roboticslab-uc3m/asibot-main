
#include <yarp/os/BufferedPort.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/Event.h>
#include <yarp/os/Time.h>
#include <yarp/os/Stamp.h>
#include <yarp/sig/Vector.h>

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/Wrapper.h>
#include <yarp/dev/PreciselyTimed.h>
#include <yarp/dev/CartesianControl.h>

#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainiksolverpos_nr_jl.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/frames.hpp>
#include <kdl/path_line.hpp>
#include <kdl/rotational_interpolation_sa.hpp>
#include <kdl/velocityprofile_rect.hpp>
#include <kdl/velocityprofile_trap.hpp>
#include <kdl/trajectory_segment.hpp>

#include "kinematics.h"

#define THREAD_RATE 50  // In miliseconds
#define GAIN 75
#define TIMEINCREMENT 0.05  // For Trajectory, 50ms?
#define CARTPOS_PRECISION 0.001  // Meter
#define CARTORI_PRECISION 1.5  // Degrees
#define DURATION 3  // For Trajectory, 3s?

using namespace yarp::os;
using namespace yarp::dev;

using namespace KDL;

class RobotControl : public DeviceDriver, public ICartesianControl, public RateThread, public Kinematics
{
public:

  RobotControl() : RateThread(THREAD_RATE) { }

  /** The periodical function **/
  void run() {
    if(cmc_status>0){  // If it is movement
      double encs[num_motors];
      enc->getEncoders(encs);
      for(int motor=0;motor<num_motors;motor++)
        real_rad(motor)=toRad(encs[motor]);
      ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
      ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
      fksolver.JntToCart(real_rad,real_cartpos);
      if(targetReached(seekOri)){ //depends
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
        
        for(unsigned int i=0; i<6; i++) {
          T_current(i) *= GAIN;
          T_current(i) += T_desired(i);  // ... T_current(i) += T_other(i);
        }

        JntArray joint_vel(num_motors);  // in radians
        ChainIkSolverVel_pinv iksolverv_pinv(theChain);
        int return_code = iksolverv_pinv.CartToJnt(real_rad,T_current,joint_vel);
        double cmc_qdot[num_motors];
        for(unsigned int i=0;i<num_motors;i++)
          cmc_qdot[i]=toDeg(joint_vel(i));
        // Should normalize anyway (Find out the max, etc)
        double max_qdot = 100;
        for(int motor=0;motor<num_motors;motor++)
          if (fabs(cmc_qdot[motor])>max_qdot)
            max_qdot = fabs(cmc_qdot[motor]);
        if (max_qdot > 100)
          for(int motor=0;motor<num_motors;motor++)
            cmc_qdot[motor]=(cmc_qdot[motor])*100.0/max_qdot;
        vel->velocityMove(cmc_qdot);
        currentTime = currentTime + TIMEINCREMENT;
      }
    } else {  // If it is stopped or breaked, reamain unchanged
      //printf("Inside control loop stopped.\n");
    }
  }

  /**
    * Set the controller in tracking or non-tracking mode. [wait for
    * reply] 
    * @param f true for tracking mode, false otherwise. 
    * @return true/false on success/failure. 
    *  
    * @note In tracking mode when the controller reachs the target, 
    *       it keeps on running in order to mantain the limb in the
    *       desired pose. In non-tracking mode the controller
    *       releases the limb as soon as the desired pose is
    *       reached.
    */
  virtual bool setTrackingMode(const bool f) {
    return true;
  }


  /**
    * Get the current controller mode. [wait for reply]
    * @param f here is returned true if controller is in tracking 
    *         mode, false otherwise.
    * @return true/false on success/failure. 
    */
  virtual bool getTrackingMode(bool *f) {
    return true;
  }

  /**
  * Get the current pose of the end-effector. [do not wait for 
  * reply] 
  * @param x a 3-d vector which is filled with the actual 
  *         position x,y,z (meters).
  * @param od a 4-d vector which is filled with the actual 
  * orientation using axis-angle representation xa, ya, za, theta 
  * (meters and radians). 
  * @return true/false on success/failure.
  */
  virtual bool getPose(yarp::sig::Vector &x, yarp::sig::Vector &o) {
    double encs[num_motors];
    enc->getEncoders(encs);
    for(int motor=0;motor<num_motors;motor++)
      real_rad(motor)=toRad(encs[motor]);
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain); // Does Forward Kinematics
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver
    fksolver.JntToCart(real_rad,real_cartpos);    
    x.resize(3);
    x[0] = real_cartpos.p.data[0];
    x[1] = real_cartpos.p.data[1];
    x[2] = real_cartpos.p.data[2];
    o.resize(4);
    KDL::Vector rotVector = real_cartpos.M.GetRot();
    o[3] = real_cartpos.M.GetRotAngle(rotVector);  // use this to normalize!!
    o[0] = rotVector[0];
    o[1] = rotVector[1];
    o[2] = rotVector[2];
    return true;
  }

  /**
    * Get the current pose of the specified link belonging to the 
    * kinematic chain. [wait for reply] 
    * @param axis joint index (regardless if it is actuated or 
    *            not). 
    * @param x a 3-d vector which is filled with the actual position
    *         x,y,z (meters) of the given link reference frame.
    * @param od a 4-d vector which is filled with the actual 
    * orientation of the given link reference frame using axis-angle
    * representation xa, ya, za, theta (meters and radians). 
    * @return true/false on success/failure.
    */
  virtual bool getPose(const int axis, yarp::sig::Vector &x, yarp::sig::Vector &o) {
    return true;
  }

  /**
  * Move the end-effector to a specified pose (position
  * and orientation) in cartesian space. [do not wait for reply]
  * @param xd a 3-d vector which contains the desired position 
  *           x,y,z
  * @param od a 4-d vector which contains the desired orientation
  * using axis-angle representation (xa, ya, za, theta). 
  * @param t set the trajectory duration time (seconds). If t<=0 
  *         (as by default) the current execution time is kept.
  * @return true/false on success/failure. 
  *  
  * @note Intended for streaming mode. 
  */
  virtual bool goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                        const double t=0.0) {
    double encs[num_motors];
    enc->getEncoders(encs);
    for(int motor=0;motor<num_motors;motor++)
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

  /**
  * Move the end-effector to a specified position in cartesian 
  * space, ignore the orientation. [do not wait for reply] 
  * @param xd a 3-d vector which contains the desired position 
  *          x,y,z (meters).
  * @param t set the trajectory duration time (seconds). If t<=0 
  *         (as by default) the current execution time is kept. 
  * @return true/false on success/failure. 
  *  
  * @note Intended for streaming mode. 
  */
  virtual bool goToPosition(const yarp::sig::Vector &xd, const double t=0.0) {
    double encs[num_motors];
    enc->getEncoders(encs);
    for(int motor=0;motor<num_motors;motor++)
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

  /**
    * Move the end-effector to a specified pose (position
    * and orientation) in cartesian space. [wait for reply]
    * @param xd a 3-d vector which contains the desired position 
    *          x,y,z (meters).
    * @param od a 4-d vector which contains the desired orientation
    * using axis-angle representation (xa, ya, za, theta). 
    * @param t set the trajectory duration time (seconds). If t<=0 
    *         (as by default) the current execution time is kept.
    * @return true/false on success/failure.
    */
  virtual bool goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                            const double t=0.0) {
    return true;
  }

  /**
  * Move the end-effector to a specified position in cartesian 
  * space, ignore the orientation. [wait for reply] 
  * @param xd a 3-d vector which contains the desired position 
  *          x,y,z (meters).
  * @param t set the trajectory duration time (seconds). If t<=0 
  *         (as by default) the current execution time is kept. 
  * @return true/false on success/failure.
  */
  virtual bool goToPositionSync(const yarp::sig::Vector &xd, const double t=0.0) {
    goToPosition(xd);
    while(cmc_status!=0); //TODO: Must timeout!!
    return true;
  }

  /**
    * Get the actual desired pose and joints configuration as result
    * of kinematic inversion. [wait for reply] 
    * @param xdhat a 3-d vector which is filled with the actual 
    *          desired position x,y,z (meters); it may differ from
    *          the commanded xd.
    * @param odhat a 4-d vector which is filled with the actual 
    *          desired orientation using axis-angle representation
    *          xa, ya, za, theta (meters and radians); it may differ
    *          from the commanded od. 
    * @param qdhat the joints configuration through which the
    *             couple (xdhat,odhat) is achieved (degrees).
    * @return true/false on success/failure.
    */
  virtual bool getDesired(yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                          yarp::sig::Vector &qdhat) {
    return true;
  }

  /**
    * Ask for inverting a given pose without actually moving there.
    * [wait for reply] 
    * @param xd a 3-d vector which contains the desired 
    *          position x,y,z (meters).
    * @param od a 4-d vector which contains the desired 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians).
    * @param xdhat a 3-d vector which is filled with the final 
    *          position x,y,z (meters); it may differ from the
    *          commanded xd.
    * @param odhat a 4-d vector which is filled with the final 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians); it may differ from
    *          the commanded od.
    * @param qdhat the joints configuration through which the
    *             couple (xdhat,odhat) is achieved (degrees).
    * @return true/false on success/failure.
    */
  virtual bool askForPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                          yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                          yarp::sig::Vector &qdhat) {
    return true;
  }

  /**
    * Ask for inverting a given pose without actually moving there.
    * [wait for reply] 
    * @param q0 a vector of length DOF which contains the starting
    *           joints configuration (degrees), made compatible with
    *           the chain.
    * @param xd a 3-d vector which contains the desired 
    *          position x,y,z (meters).
    * @param od a 4-d vector which contains the desired 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians).
    * @param xdhat a 3-d vector which is filled with the final 
    *          position x,y,z (meters); it may differ from the
    *          commanded xd.
    * @param odhat a 4-d vector which is filled with the final 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians); it may differ from
    *          the commanded od.
    * @param qdhat the joints configuration through which the
    *             couple (xdhat,odhat) is achieved (degrees).
    * @return true/false on success/failure.
    */
  virtual bool askForPose(const yarp::sig::Vector &q0,
                          const yarp::sig::Vector &xd, const yarp::sig::Vector &od,
                          yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                          yarp::sig::Vector &qdhat) {
    return true;
  }

  /**
    * Ask for inverting a given position without actually moving 
    * there. [wait for reply] 
    * @param xd a 3-d vector which contains the desired 
    *          position x,y,z (meters).
    * @param xdhat a 3-d vector which is filled with the final 
    *          position x,y,z (meters); it may differ from the
    *          commanded xd.
    * @param odhat a 4-d vector which is filled with the final 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians); it may differ from
    *          the commanded od.
    * @param qdhat the joints configuration through which the
    *             couple (xdhat,odhat) is achieved (degrees).
    * @return true/false on success/failure.
    */
  virtual bool askForPosition(const yarp::sig::Vector &xd,
                              yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                              yarp::sig::Vector &qdhat) {
    return true;
  }

  /**
    * Ask for inverting a given position without actually moving 
    * there. [wait for reply] 
    * @param q0 a vector of length DOF which contains the starting 
    *           joints configuration (degrees), made compatible with
    *           the chain.
    * @param xd a 3-d vector which contains the desired 
    *          position x,y,z (meters).
    * @param xdhat a 3-d vector which is filled with the final 
    *          position x,y,z (meters); it may differ from the
    *          commanded xd.
    * @param odhat a 4-d vector which is filled with the final 
    *          orientation using axis-angle representation xa, ya,
    *          za, theta (meters and radians); it may differ from
    *          the commanded od.
    * @param qdhat the joints configuration through which the
    *             couple (xdhat,odhat) is achieved (degrees).
    * @return true/false on success/failure.
    */
  virtual bool askForPosition(const yarp::sig::Vector &q0,
                              const yarp::sig::Vector &xd,
                              yarp::sig::Vector &xdhat, yarp::sig::Vector &odhat,
                              yarp::sig::Vector &qdhat) {
    return true;
  }

  /**
    * Get the current DOF configuration of the limb. [wait for
    * reply] 
    * @param curDof a vector which is filled with the actual DOF 
    *           configuration.
    * @return true/false on success/failure. 
    *  
    * @note The vector lenght is equal to the number of limb's 
    *       joints; each vector's position is filled with 1 if the
    *       associated joint is controlled (i.e. it is an actuated
    *       DOF), 0 otherwise.
    */
  virtual bool getDOF(yarp::sig::Vector &curDof) {
    return true;
  }

  /**
    * Set a new DOF configuration for the limb. [wait for reply]
    * @param newDof a vector which contains the new DOF 
    *            configuration.
    * @param curDof a vector where the DOF configuration is 
    *              returned as it has been processed after the
    *              request (it may differ from newDof due to the
    *              presence of some internal limb's constraints).
    * @return true/false on success/failure. 
    *  
    * @note Eeach vector's position shall contain 1 if the 
    *       associated joint can be actuated, 0 otherwise. The
    *       special value 2 indicates that the joint status won't be
    *       modified (useful as a placeholder).
    */
  virtual bool setDOF(const yarp::sig::Vector &newDof, yarp::sig::Vector &curDof) {
    return true;
  }

  /**
    * Get the current joints rest position. [wait for reply]
    * @param curRestPos a vector which is filled with the current 
    *                  joints rest position components in degrees.
    * @return true/false on success/failure. 
    *  
    * @note While solving the inverse kinematic, the user may 
    *       specify a secondary task that minimizes against a joints
    *       rest position; further, each rest component may be
    *       weighted differently providing the weights vector.
    */
  virtual bool getRestPos(yarp::sig::Vector &curRestPos) {
    return true;
  }

  /**
    * Set a new joints rest position. [wait for reply] 
    * @param newRestPos a vector which contains the new joints rest
    *                  position components in degrees.
    * @param curRestPos a vector which is filled with the current 
    *           joints rest position components in degrees as result
    *           from thresholding with joints bounds.
    * @return true/false on success/failure. 
    *  
    * @note While solving the inverse kinematic, the user may 
    *       specify a secondary task that minimizes against a joints
    *       rest position; further, each rest component may be
    *       weighted differently providing the weights vector.
    */
  virtual bool setRestPos(const yarp::sig::Vector &newRestPos, yarp::sig::Vector &curRestPos) {
    return true;
  }

  /**
    * Get the current joints rest weights. [wait for reply]
    * @param curRestWeights a vector which is filled with the 
    *                  current joints rest weights.
    * @return true/false on success/failure. 
    *  
    * @note While solving the inverse kinematic, the user may 
    *       specify a secondary task that minimizes against a joints
    *       rest position; further, each rest component may be
    *       weighted differently providing the weights vector.
    */
  virtual bool getRestWeights(yarp::sig::Vector &curRestWeights) {
    return true;
  }

  /**
    * Set a new joints rest position. [wait for reply] 
    * @param newRestWeights a vector which contains the new joints 
    *                  rest weights.
    * @param curRestWeights a vector which is filled with the 
    *           current joints rest weights as result from
    *           saturation (w>=0.0).
    * @return true/false on success/failure. 
    *  
    * @note While solving the inverse kinematic, the user may 
    *       specify a secondary task that minimizes against a joints
    *       rest position; further, each rest component may be
    *       weighted differently providing the weights vector.
    */
  virtual bool setRestWeights(const yarp::sig::Vector &newRestWeights, yarp::sig::Vector &curRestWeights) {
    return true;
  }

  /**
    * Get the current range for the axis. [wait for reply]
    * @param axis joint index (regardless if it is actuated or 
    *            not).
    * @param min where the minimum value is returned [deg].
    * @param max where the maximum value is returned [deg].
    * @return true/false on success/failure. 
    */
  virtual bool getLimits(const int axis, double *min, double *max) {
    return true;
  }

  /**
    * Set new range for the axis. Allowed range shall be a valid 
    * subset of the real control limits. [wait for reply]
    * @param axis joint index (regardless it it is actuated or 
    *            not).
    * @param min the new minimum value [deg]. 
    * @param max the new maximum value [deg]. 
    * @return true/false on success/failure. 
    */
  virtual bool setLimits(const int axis, const double min, const double max) {
    return true;
  }

  /**
    * Get the current trajectory duration. [wait for reply]
    * @param t time (seconds).
    * @return true/false on success/failure. 
    */
  virtual bool getTrajTime(double *t) {
    return true;
  }

  /**
    * Set the duration of the trajectory. [wait for reply]
    * @param t the memory location where the time is returned 
    *         (seconds).
    * @return true/false on success/failure. 
    */
  virtual bool setTrajTime(const double t) {
    return true;
  }

  /**
    * Return tolerance for in-target check. [wait for reply]
    * @param tol the memory location where tolerance is returned. 
    * @return true/false on success/failure. 
    *  
    * @note The trajectory is supposed to be completed as soon as 
    *       norm(xd-end_effector)<tol.
    */
  virtual bool getInTargetTol(double *tol) {
    return true;
  }

  /**
    * Set tolerance for in-target check. [wait for reply]
    * @param tol tolerance. 
    * @return true/false on success/failure. 
    *  
    * @note The trajectory is supposed to be completed as soon as 
    *       norm(xd-end_effector)<tol.
    */
  virtual bool setInTargetTol(const double tol) {
    return true;
  }

  /**
    * Return joints velocities. [wait for reply] 
    * @param qdot the vector containing the joints velocities 
    *             [deg/s] sent to the robot by the controller.
    * @return true/false on success/failure.
    */
  virtual bool getJointsVelocities(yarp::sig::Vector &qdot) {
    return true;
  }

  /**
    * Return velocities of the end-effector in the task space. [wait
    * for reply] 
    * @param xdot the 3-d vector containing the derivative of x,y,z 
    *             position [m/s] of the end-effector while moving in
    *             the task space as result of the commanded joints
    *             velocities.
    * @param odot the 4-d vector containing the derivative of 
    *             end-effector orientation [rad/s] while moving in
    *             the task space as result of the commanded joints
    *             velocities.
    * @return true/false on success/failure.
    */
  virtual bool getTaskVelocities(yarp::sig::Vector &xdot, yarp::sig::Vector &odot) {
    return true;
  }

  /** Check once if the current trajectory is terminated. [wait for
    *   reply]
    * @param f where the result is returned.
    * @return true/false on success/failure.
    */
  virtual bool checkMotionDone(bool *f) {
    return true;
  }

  /** Wait until the current trajectory is terminated. [wait for
    *   reply]
    * @param period specify the check time period (seconds). 
    * @param timeout specify the check expiration time (seconds). If
    *         timeout<=0 (as by default) the check will be performed
    *         without time limitation.
    * @return true for success, false for failure and timeout 
    *         expired.
    */
  virtual bool waitMotionDone(const double period=0.1, const double timeout=0.0) {
    return true;
  }

  /** Ask for an immediate stop motion. [wait for reply]
    * @return true/false on success/failure. 
    *  
    * @note The control is completely released, i.e. a direct switch
    *       to non-tracking mode is executed.     
    */
  virtual bool stopControl() {
    cmc_status=0;
    vel->stop(); // Set vel of all motors to 0
    return true;
  }

  /** Store the controller context. [wait for reply]
    * @param id specify where to store the returned context id. 
    * @return true/false on success/failure. 
    *  
    * @note The context comprises the values of internal controller 
    *       variables, such as the tracking mode, the active dofs,
    *       the trajectory time and so on.
    */
  virtual bool storeContext(int *id) {
    return true;
  }

  /** Restore the controller context previously stored. [wait for
    *   reply]
    * @param id specify the context id to be restored
    * @return true/false on success/failure. 
    *  
    * @note The context comprises the values of internal controller
    *       variables, such as the tracking mode, the active dofs,
    *       the trajectory time and so on.
    */
  virtual bool restoreContext(const int id) {
    return true;
  }

  virtual bool setTaskVelocities(const yarp::sig::Vector&, const yarp::sig::Vector&) {
    return true;
  }

  bool open(Searchable &config) {

    printf("================================\n");
    printf("check our device can be accessed\n");

    Property options;

    options.put("device","remote_controlboard");
    options.put("remote","/ravebot");
    options.put("local","/robotcontrol/ravebot");

    dd.open(options);
    
    if (!dd.isValid()) {
      printf("[error] remote_controlboard not valid.\n\n");
      return false;
    }

    dd.view(pos);
    pos->getAxes(&num_motors);
    printf("Going to control %d motors.\n",num_motors);

    dd.view(enc);
    dd.view(vel);
    //pos->positionMove(0, 0);
    //pos->positionMove(1, -45);
    //pos->positionMove(2, 90);
    //pos->positionMove(3, 45);
    //pos->positionMove(4, 0);

    this->start();

    return true;
  }

  /** When we start the periodical function **/
  bool threadInit(){
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

  bool targetReached(bool withOri){
    if(fabs((real_cartpos.p.data[0])-(target_cartpos.p.data[0]))>CARTPOS_PRECISION) return false;
    if(fabs((real_cartpos.p.data[1])-(target_cartpos.p.data[1]))>CARTPOS_PRECISION) return false;
    if(fabs((real_cartpos.p.data[2])-(target_cartpos.p.data[2]))>CARTPOS_PRECISION) return false;
    double target_rpy[3] = {0,0,0};
    double real_rpy[3] = {0,0,0};
    real_cartpos.M.GetRPY(real_rpy[0],real_rpy[1],real_rpy[2]);
    target_cartpos.M.GetRPY(target_rpy[0],target_rpy[1],target_rpy[2]);
    if((withOri)&&(fabs(toDeg(target_rpy[0])-toDeg(real_rpy[0])))>CARTORI_PRECISION) return false;
    if((withOri)&&(fabs(toDeg(target_rpy[1])-toDeg(real_rpy[1])))>CARTORI_PRECISION) return false;
    if((withOri)&&(fabs(toDeg(target_rpy[2])-toDeg(real_rpy[2])))>CARTORI_PRECISION) return false;
    return true;
  }

  double toDeg(const double inRad){
    return (inRad * 180.0 / 3.14159265);
  }

  double toRad(const double inDeg){
    return (inDeg * 3.14159265 / 180.0);
  }

private:
  PolyDriver dd; // Closes if out of scope
  IEncoders *enc;
  IPositionControl *pos;
  IVelocityControl *vel;
  Chain theChain;
  Frame real_cartpos;
  Frame target_cartpos;
  JntArray real_rad;  // in radians
  int num_motors;
  RotationalInterpolation_SingleAxis* _orient;
  double _eqradius;
  bool _aggregate;
  Trajectory* currentTrajectory;
  double currentTime;
  double vgeneral;
  int cmc_status;
  bool seekOri;
};


