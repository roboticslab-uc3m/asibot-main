// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <yarp/os/all.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/all.h>

#include <iostream> // only windows

#include <stdlib.h> // for exit()

#define DEFAULT_NUM_MOTORS 6
#define MAX_NUM_MOTORS 50
#define THREAD_RATE 5  // In miliseconds.
#define MOTOR_PRECISION 0.5  // In degrees.
#define SPEED_ADJ 0.05  // Speed adjustment for simulation
#define NEG_LIM -180.0  // NOT IMPLEMENTED
#define POS_LIM 180.0  // NOT IMPLEMETED

using namespace std;

using namespace yarp::os;
using namespace yarp::dev;

/**
 *
 * @ingroup CartesianBot
 *
 * CartesianBot creates a virtual (text-mode) N-DOF robot and exposes a YARP_dev controlboard
 * interface (implements the IPositionControl, IVelocityControl and IEncoders interfaces).
 *
 * <b>Installation</b>
 *
 * The plugin is compiled when ENABLE_rlPlugins_cartesianbot is activated (not default). For further
 * installation steps refer to <a href="pages.html">your own system installation guidelines</a>.
 *
 */
class CartesianBot : public DeviceDriver, public RateThread, public IPositionControl, public IVelocityControl, public IEncoders {
 public:

  // Set the Thread Rate in the class constructor
  CartesianBot() : RateThread(THREAD_RATE) {}  // In ms

 // ------------------ IPositionControl Related -------------------------------
  /**
   * Get the number of controlled axes. This command asks the number of controlled
   * axes for the current physical interface.
   * @param ax pointer to storage
   * @return true/false.
   */
  virtual bool getAxes(int *ax) {
    *ax = num_motors;
    printf("RaveBot reporting %d axes are present\n", *ax);
    return true;
  }

  /** Set position mode. This command
   * is required by control boards implementing different
   * control methods (e.g. velocity/torque), in some cases
   * it can be left empty.
   * return true/false on success/failure
   */
  virtual bool setPositionMode() {
    return true;  // Not implemented, as [jgvictores] does not understand (yet).
  }

  /** Set new reference point for a single axis.
   * @param j joint number
   * @param ref specifies the new ref point
   * @return true/false on success/failure
   */
  virtual bool positionMove(int j, double ref) {
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=1;
    target_degrees[j]=ref;
    if (ref>real_degrees[j]) joint_vel[j] = (THREAD_RATE*SPEED_ADJ*gvels[j])/100.0;
    else joint_vel[j] = -(THREAD_RATE*SPEED_ADJ*gvels[j])/100.0;
    return true;
  }


  /** Set new reference point for all axes.
   * @param refs array, new reference points.
   * @return true/false on success/failure
   */
  virtual bool positionMove(const double *refs) {
    // Find out the maximum angle to move
    double max_dist = 0;
    for(int motor=0;motor<num_motors;motor++)
      if (fabs(refs[motor]-real_degrees[motor])>max_dist)
        max_dist = fabs(refs[motor]-real_degrees[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0;motor<num_motors;motor++) {
      joint_status[motor]=1;
      target_degrees[motor]=refs[motor];
      joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(refs[motor]-real_degrees[motor])/max_dist)/(100.0);
    }
    return true;
  }


  /** Set relative position. The command is relative to the 
   * current position of the axis.
   * @param j joint axis number
   * @param delta relative command
   * @return true/false on success/failure
   */
  virtual bool relativeMove(int j, double delta) {
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    joint_status[j]=2;
    target_degrees[j]=real_degrees[j]+delta;
    if (delta>0) joint_vel[j] = (THREAD_RATE*SPEED_ADJ*gvels[j])/(100.0);
    else joint_vel[j] = -(THREAD_RATE*SPEED_ADJ*gvels[j])/(100.0);
    return true;
  }


  /** Set relative position, all joints.
   * @param deltas pointer to the relative commands
   * @return true/false on success/failure
   */
  virtual bool relativeMove(const double *deltas) {
    // Find out the maximum angle to move
    double max_dist = 0;
    for(int motor=0;motor<num_motors;motor++)
      if (fabs(deltas[motor])>max_dist)
        max_dist = fabs(deltas[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0; motor<num_motors; motor++) {
      joint_status[motor]=2;
      target_degrees[motor]=real_degrees[motor]+deltas[motor];
      joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(deltas[motor])/max_dist)/(100.0);
    }
    return true;
  }


  /** Check if the current trajectory is terminated. Non blocking.
   * @return true if the trajectory is terminated, false otherwise
   */
  virtual bool checkMotionDone(int j, bool *flag) {
    if (joint_status[j]<=0) *flag=true;
    else *flag=false;
    return true;
  }


  /** Check if the current trajectory is terminated. Non blocking.
   * @return true if the trajectory is terminated, false otherwise
   */
  virtual bool checkMotionDone(bool *flag) {
    for (unsigned int i=0; i<num_motors; i++) {
      if (joint_status[i]<=0) flag[i]=true;
      else flag[i]=false;      
    }
    return true;
  }


  /** Set reference speed for a joint, this is the speed used during the
   * interpolation of the trajectory.
   * @param j joint number
   * @param sp speed value
   * @return true/false upon success/failure
   */
  virtual bool setRefSpeed(int j, double sp) {
    if (sp>100) gvels[j]=100;
    else if (sp<-100) gvels[j]=-100;
    else gvels[j]=sp;
    return true;
  }


  /** Set reference speed on all joints. These values are used during the
   * interpolation of the trajectory.
   * @param spds pointer to the array of speed values.
   * @return true/false upon success/failure
   */
  virtual bool setRefSpeeds(const double *spds) {
    for (unsigned int i=0; i<num_motors; i++) {
      if (spds[i]>100) gvels[i]=100;
      else if (spds[i]<-100) gvels[i]=-100;
      else gvels[i]=spds[i];
    }
    return true;
  }


  /** Set reference acceleration for a joint. This value is used during the
   * trajectory generation.
   * @param j joint number
   * @param acc acceleration value
   * @return true/false upon success/failure
   */
  virtual bool setRefAcceleration(int j, double acc) {
    return true;
  }


  /** Set reference acceleration on all joints. This is the valure that is
   * used during the generation of the trajectory.
   * @param accs pointer to the array of acceleration values
   * @return true/false upon success/failure
   */
  virtual bool setRefAccelerations(const double *accs) {
    return true;  // Not implemented, its unlimited for now
  }


  /** Get reference speed for a joint. Returns the speed used to 
   * generate the trajectory profile.
   * @param j joint number
   * @param ref pointer to storage for the return value
   * @return true/false on success or failure
   */
  virtual bool getRefSpeed(int j, double *ref) {
    *ref=gvels[j];   
    return true;
  }


  /** Get reference speed of all joints. These are the  values used during the
   * interpolation of the trajectory.
   * @param spds pointer to the array that will store the speed values.
   */
  virtual bool getRefSpeeds(double *spds) {
    for (unsigned int i=0; i<num_motors; i++)
      spds[i]=gvels[i];
    return true;
  }


  /** Get reference acceleration for a joint. Returns the acceleration used to 
   * generate the trajectory profile.
   * @param j joint number
   * @param acc pointer to storage for the return value
   * @return true/false on success/failure
   */
  virtual bool getRefAcceleration(int j, double *acc) {
    return true;
  }


  /** Get reference acceleration of all joints. These are the values used during the
   * interpolation of the trajectory.
   * @param accs pointer to the array that will store the acceleration values.
   * @return true/false on success or failure 
   */
  virtual bool getRefAccelerations(double *accs) {
    return true;
  }


  /** Stop motion, single joint
   * @param j joint number
   * @return true/false on success/failure
   */
  virtual bool stop(int j) {
    joint_vel[j]=0.0;
    joint_status[j]=-1;
    return true;
  }


  /** Stop motion, multiple joints 
   * @return true/false on success/failure
   */
  virtual bool stop() {
    for (unsigned int i=0; i<num_motors; i++) {
      joint_vel[i]=0.0;
      joint_status[i]=-1;
    }
    return true;
  }

 // ------------------ IEncoder Related ---------------------------------------
  /**
   * Reset encoder, single joint. Set the encoder value to zero 
   * @param j encoder number
   * @return true/false
   */
  virtual bool resetEncoder(int j) {
    real_degrees[j]=0.0;
    return true;
  }

  /**
   * Reset encoders. Set the encoders value to zero 
   * @return true/false
   */
  virtual bool resetEncoders() {
    for (unsigned int i=0; i<num_motors; i++)
      real_degrees[i]=0.0;
    return true;
  }


  /**
   * Set the value of the encoder for a given joint. 
   * @param j encoder number
   * @param val new value
   * @return true/false
   */
  virtual bool setEncoder(int j, double val) {
    real_degrees[j]=val;
    return true;
  }


  /**
   * Set the value of all encoders.
   * @param vals pointer to the new values
   * @return true/false
   */
  virtual bool setEncoders(const double *vals) {
    for (unsigned int i=0; i<num_motors; i++)
      real_degrees[i]=vals[i];
    return true;
  }


  /**
   * Read the value of an encoder.
   * @param j encoder number
   * @param v pointer to storage for the return value
   * @return true/false, upon success/failure (you knew it, uh?)
   */
  virtual bool getEncoder(int j, double *v) {
    *v=real_degrees[j];
    return true;
  }

  /**
   * Read the position of all axes.
   * @param encs pointer to the array that will contain the output
   * @return true/false on success/failure
   */
  virtual bool getEncoders(double *encs) {
    for (unsigned int i=0; i<num_motors; i++)
      encs[i]=real_degrees[i];
    return true;
  }


  /**
   * Read the istantaneous speed of an axis.
   * @param j axis number
   * @param sp pointer to storage for the output
   * @return true if successful, false ... otherwise.
   */
  virtual bool getEncoderSpeed(int j, double *sp) {
    *sp=joint_vel[j];
    return true;
  }

  /**
   * Read the instantaneous speed of all axes.
   * @param spds pointer to storage for the output values
   * @return guess what? (true/false on success or failure).
   */
  virtual bool getEncoderSpeeds(double *spds) {
    for (unsigned int i=0; i<num_motors; i++)
      spds[i]=joint_vel[i];
    return true;
  }

    
  /**
   * Read the instantaneous acceleration of an axis.
   * @param j axis number
   * @param spds pointer to the array that will contain the output
   */
  virtual bool getEncoderAcceleration(int j, double *spds) {
    return true;
  }

  /**
   * Read the instantaneous acceleration of all axes.
   * @param accs pointer to the array that will contain the output
   * @return true if all goes well, false if anything bad happens. 
   */
  virtual bool getEncoderAccelerations(double *accs) {
    return true;
  }


 // ------------------ IVelocityControl Related -------------------------------
  /**
   * Set position mode. This command
   * is required by control boards implementing different
   * control methods (e.g. velocity/torque), in some cases
   * it can be left empty.
   * @return true/false on success failure
   */
  virtual bool setVelocityMode()  {
    return true;  // Not implemented, as [jgvictores] does not understand (yet).
  }


  /**
   * Start motion at a given speed, single joint.
   * @param j joint number
   * @param sp speed value
   * @return bool/false upone success/failure
   */
  virtual bool velocityMove(int j, double sp) {
    if(sp>0) target_degrees[j]=180.0; // Must correct for JL
    else target_degrees[j]=-180.0;
    if (sp>100) sp=100;
    else if (sp<-100) sp=-100;
//    gvels[j]=sp;
//    joint_vel[motor] = (THREAD_RATE*SPEED_ADJ*gvels[motor]*(deltas[motor])/max_dist)/(100.0);
    joint_vel[j] = (THREAD_RATE*SPEED_ADJ*sp)/100.0;
    joint_status[j]=3;
    return true;
  }

  /**
   * Start motion at a given speed, multiple joints.
   * @param sp pointer to the array containing the new speed values
   * @return true/false upon success/failure
   */
  virtual bool velocityMove(const double *sp) {
    double sp_limited[MAX_NUM_MOTORS];
    for (unsigned int i=0; i<num_motors; i++) {
      if(sp[i]>0) target_degrees[i]=POS_LIM; // Must correct for JL
      else target_degrees[i]=NEG_LIM;
      if (sp[i]>100) sp_limited[i]=100;
      else if (sp[i]<-100) sp_limited[i]=-100;
      else sp_limited[i]=sp[i];
      joint_vel[i] = (THREAD_RATE*SPEED_ADJ*sp_limited[i])/100.0;
      joint_status[i]=3;
    }
    return true;
  }


 // ------------------- DeviceDriver Related ------------------------------------
  virtual bool open(Searchable& config) {
    printf("HI I WORK!!!!!\n");

    if(config.check("help")) {
       printf("\n");
       printf("Usage: cartesianbot --help  -------> This help\n");
       printf("Usage: cartesianbot --DOF [DOF]  --> [DOF] degrees of freedom (defaults to %d, max %d)\n\n",DEFAULT_NUM_MOTORS,MAX_NUM_MOTORS);
       exit(1);
    }

    Value v = config.check("DOF",Value(DEFAULT_NUM_MOTORS));
    num_motors=v.asInt();
    if(num_motors>MAX_NUM_MOTORS) {
      printf("Exceeded max DOF. ");
      num_motors=MAX_NUM_MOTORS;
    }
    printf("Using %d DOF.\n",v.asInt());  

    // Initialize private parameters
    for (unsigned int i=0; i<num_motors; i++) {
      joint_status[i]=0;
      real_degrees[i]=0.0;
      joint_vel[i]=0.0;
      target_degrees[i]=0.0;
      gvels[i]=100.0;
    }

    // Start the RateThread
    this->start();
    
    return true;
  }


  virtual bool close() {
    return true;
  }


 // ------------------- RateThread Related ------------------------------------
  /** On the thread initialization **/
  bool threadInit() {
    printf("threadInit(): ok\n");
    return true;
  }


  /** The periodical function **/
  void run() {
    for(int motor=0;motor<num_motors;motor++){
      if((joint_status[motor]==1)||(joint_status[motor]==2)||(joint_status[motor]==3)||(joint_status[motor]==5)) {
        if (fabs(target_degrees[motor]-real_degrees[motor])<MOTOR_PRECISION){
          printf("Joint q%d reached target.\n",motor+1);
          joint_status[motor]=0;
          joint_vel[motor]=0;
        // Here we should check for joint limits
        } else {
          real_degrees[motor]+=joint_vel[motor];
        }
      }
    }
    for (unsigned int i=0; i<num_motors; i++) 
      printf("%f ",real_degrees[i]);
    printf("\n");
  }

 private:
  // General Joint Motion Controller parameters //
  int num_motors;
  int joint_status[MAX_NUM_MOTORS];
  double real_degrees[MAX_NUM_MOTORS];
  double target_degrees[MAX_NUM_MOTORS];
  double joint_vel[MAX_NUM_MOTORS];
  double gvels[MAX_NUM_MOTORS];
  // YARP
};


