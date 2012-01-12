// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <yarp/os/all.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/all.h>

#include <openrave-core.h>
//#include <openrave/kinbody.h>

#include <stdio.h>
#include <sstream>

#define NUM_MOTORS 5
#define THREAD_RATE 0.5  // In miliseconds.
#define MOTOR_PRECISION 0.5  // In degrees. Was .75
#define SPEED_ADJ 0.2  // Speed adjustment for simulation, pos.
#define SPEED_ADJ_V 0.3  // Speed adjustment for simulation, vel.
#define NEG_LIM -135.0
#define POS_LIM 135.0
#define NEG_LIM_EXT -180.0
#define POS_LIM_EXT 180.0
#define MI_PI 3.14159265
#define UNSTABLE false

#define DEFAULT_ENV_NAME "/app/models/asibot_cocina_entero.env.xml"  // on $ASIBOT_ROOT

using namespace std;

using namespace yarp::os;
using namespace yarp::dev;

using namespace OpenRAVE;

class toolPort : public BufferedPort<Bottle> {
  public:
    toolPort() : status(0) {}
    double target;
    bool status;
  private:
    virtual void onRead(Bottle& b) {
        printf("[toolPort::callback] Got %s, ", b.toString().c_str());
        if (b.get(0).getCode() == BOTTLE_TAG_DOUBLE) {
            target = b.get(0).asDouble();
            status = 1;
        }
    }
};

/**
 * 
 * @ingroup RaveBot
 *
 * \image html ravebot.png
 *
 * RaveBot creates an instance of OpenRAVE-core (qtcoin viewer included) and implements the
 * YARP_dev IPositionControl, IVelocityControl and IEncoders interface class member functions.
 * It additionally publishes the stream of images from a camera set on robot[0] (default image
 * port name: /ravebot/img:o). It is used by <a href="group__test__kdl.html">test_kdl</a> and
 * <a href="group__test__ravebot.html">test_ravebot</a>.
 *
 * <b>Installation</b>
 *
 * The plugin is compiled when ENABLE_rlPlugins_ravebot is activated (not default). For further
 * installation steps refer to <a href="pages.html">your own system installation guidelines</a>.
 *
 */
class RaveBot : public DeviceDriver, public RateThread, public IPositionControl, public IVelocityControl, public IEncoders {
 public:

  // Set the Thread Rate in the class constructor
  RaveBot() : RateThread(THREAD_RATE) {}  // In ms

  /**
   * --- IPositionControl declarations. Implementation in IPositionImpl.cpp ---
   */

  /**
   * Get the number of controlled axes. This command asks the number of controlled
   * axes for the current physical interface.
   * @param ax pointer to storage
   * @return true/false.
   */
  virtual bool getAxes(int *ax);

  /** Set position mode. This command
   * is required by control boards implementing different
   * control methods (e.g. velocity/torque), in some cases
   * it can be left empty.
   * return true/false on success/failure
   */
  virtual bool setPositionMode();

  /** Set new reference point for a single axis.
   * @param j joint number
   * @param ref specifies the new ref point
   * @return true/false on success/failure
   */
  virtual bool positionMove(int j, double ref);

  /** Set new reference point for all axes.
   * @param refs array, new reference points.
   * @return true/false on success/failure
   */
  virtual bool positionMove(const double *refs);

  /** Set relative position. The command is relative to the 
   * current position of the axis.
   * @param j joint axis number
   * @param delta relative command
   * @return true/false on success/failure
   */
  virtual bool relativeMove(int j, double delta);

  /** Set relative position, all joints.
   * @param deltas pointer to the relative commands
   * @return true/false on success/failure
   */
  virtual bool relativeMove(const double *deltas);

  /** Check if the current trajectory is terminated. Non blocking.
   * @return true if the trajectory is terminated, false otherwise
   */
  virtual bool checkMotionDone(int j, bool *flag);

  /** Check if the current trajectory is terminated. Non blocking.
   * @return true if the trajectory is terminated, false otherwise
   */
  virtual bool checkMotionDone(bool *flag);

  /** Set reference speed for a joint, this is the speed used during the
   * interpolation of the trajectory.
   * @param j joint number
   * @param sp speed value
   * @return true/false upon success/failure
   */
  virtual bool setRefSpeed(int j, double sp);

  /** Set reference speed on all joints. These values are used during the
   * interpolation of the trajectory.
   * @param spds pointer to the array of speed values.
   * @return true/false upon success/failure
   */
  virtual bool setRefSpeeds(const double *spds);

  /** Set reference acceleration for a joint. This value is used during the
   * trajectory generation.
   * @param j joint number
   * @param acc acceleration value
   * @return true/false upon success/failure
   */
  virtual bool setRefAcceleration(int j, double acc);

  /** Set reference acceleration on all joints. This is the valure that is
   * used during the generation of the trajectory.
   * @param accs pointer to the array of acceleration values
   * @return true/false upon success/failure
   */
  virtual bool setRefAccelerations(const double *accs);

  /** Get reference speed for a joint. Returns the speed used to 
   * generate the trajectory profile.
   * @param j joint number
   * @param ref pointer to storage for the return value
   * @return true/false on success or failure
   */
  virtual bool getRefSpeed(int j, double *ref);

  /** Get reference speed of all joints. These are the  values used during the
   * interpolation of the trajectory.
   * @param spds pointer to the array that will store the speed values.
   */
  virtual bool getRefSpeeds(double *spds);

  /** Get reference acceleration for a joint. Returns the acceleration used to 
   * generate the trajectory profile.
   * @param j joint number
   * @param acc pointer to storage for the return value
   * @return true/false on success/failure
   */
  virtual bool getRefAcceleration(int j, double *acc);

  /** Get reference acceleration of all joints. These are the values used during the
   * interpolation of the trajectory.
   * @param accs pointer to the array that will store the acceleration values.
   * @return true/false on success or failure 
   */
  virtual bool getRefAccelerations(double *accs);

  /** Stop motion, single joint
   * @param j joint number
   * @return true/false on success/failure
   */
  virtual bool stop(int j);

  /** Stop motion, multiple joints 
   * @return true/false on success/failure
   */
  virtual bool stop();

  /**
   *  --------- IEncoder Declarations. Implementation in IEncoderImpl.cpp ---------
   */

  /**
   * Reset encoder, single joint. Set the encoder value to zero 
   * @param j encoder number
   * @return true/false
   */
  virtual bool resetEncoder(int j);

  /**
   * Reset encoders. Set the encoders value to zero 
   * @return true/false
   */
  virtual bool resetEncoders();

  /**
   * Set the value of the encoder for a given joint. 
   * @param j encoder number
   * @param val new value
   * @return true/false
   */
  virtual bool setEncoder(int j, double val);

  /**
   * Set the value of all encoders.
   * @param vals pointer to the new values
   * @return true/false
   */
  virtual bool setEncoders(const double *vals);

  /**
   * Read the value of an encoder.
   * @param j encoder number
   * @param v pointer to storage for the return value
   * @return true/false, upon success/failure (you knew it, uh?)
   */
  virtual bool getEncoder(int j, double *v);

  /**
   * Read the position of all axes.
   * @param encs pointer to the array that will contain the output
   * @return true/false on success/failure
   */
  virtual bool getEncoders(double *encs);

  /**
   * Read the istantaneous speed of an axis.
   * @param j axis number
   * @param sp pointer to storage for the output
   * @return true if successful, false ... otherwise.
   */
  virtual bool getEncoderSpeed(int j, double *sp);

  /**
   * Read the instantaneous speed of all axes.
   * @param spds pointer to storage for the output values
   * @return guess what? (true/false on success or failure).
   */
  virtual bool getEncoderSpeeds(double *spds);
    
  /**
   * Read the instantaneous acceleration of an axis.
   * @param j axis number
   * @param spds pointer to the array that will contain the output
   */
  virtual bool getEncoderAcceleration(int j, double *spds);

  /**
   * Read the instantaneous acceleration of all axes.
   * @param accs pointer to the array that will contain the output
   * @return true if all goes well, false if anything bad happens. 
   */
  virtual bool getEncoderAccelerations(double *accs);


 // ------------------ IVelocityControl Related -------------------------------
  /**
   * Set position mode. This command
   * is required by control boards implementing different
   * control methods (e.g. velocity/torque), in some cases
   * it can be left empty.
   * @return true/false on success failure
   */
  virtual bool setVelocityMode()  {
    printf("RaveBot::setVelocityMode()\n");
    modePosVel = 1;
    return true;
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
    joint_vel[j] = (THREAD_RATE*SPEED_ADJ_V*sp)/(100.0);
    joint_status[j]=3;
    return true;
  }

  /**
   * Start motion at a given speed, multiple joints.
   * @param sp pointer to the array containing the new speed values
   * @return true/false upon success/failure
   */
  virtual bool velocityMove(const double *sp) {
    double sp_limited[NUM_MOTORS];
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      if(sp[i]>0) target_degrees[i]=180.0; // Must correct for JL
      else target_degrees[i]=-180.0;
      if (sp[i]>100) sp_limited[i]=100;
      else if (sp[i]<-100) sp_limited[i]=-100;
      else sp_limited[i]=sp[i];
      joint_vel[i] = (THREAD_RATE*SPEED_ADJ_V*sp_limited[i])/(100.0);
      printf("Vel %d: %f",i,joint_vel[i]);
      joint_status[i]=3;
    }
    printf("\n");
    return true;
  }


  /**
   * ------ DeviceDriver declarations. Implementation in IDeviceImpl.cpp ------
   */

  /**
   * Open the DeviceDriver. 
   * @param config is a list of parameters for the device.
   * Which parameters are effective for your device can vary.
   * See \ref dev_examples "device invocation examples".
   * If there is no example for your device,
   * you can run the "yarpdev" program with the verbose flag
   * set to probe what parameters the device is checking.
   * If that fails too,
   * you'll need to read the source code (please nag one of the 
   * yarp developers to add documentation for your device).
   * @return true/false upon success/failure
   */
  virtual bool open(Searchable& config);

  /**
   * Close the DeviceDriver.
   * @return true/false on success/failure.
   */
  virtual bool close();

 // ------------------- RateThread Related ------------------------------------
  /** On the thread initialization **/
  bool threadInit() {
    printf("threadInit(): yay!\n");
    return true;
  }


  /** The periodical function **/
  void run() {
    std::vector<dReal> next_positions(mismotores.size());
    for(int motor=0;motor<5;motor++){
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
      next_positions[motor]=float(real_degrees[motor]*MI_PI/180.0);
    }

    if (toolFound) {
        if(theToolPort.status==1) {
            if (fabs(theToolPort.target-real_tool)>MOTOR_PRECISION) {
                if(theToolPort.target-real_tool>0)
                    real_tool+=THREAD_RATE*SPEED_ADJ*0.25;
                else
                    real_tool-=THREAD_RATE*SPEED_ADJ*0.25;
                printf("Tool at %f\n",real_tool);
            } else {
                theToolPort.status = 0;
                //imlast=true;
            }
        }
        next_positions[5] = real_tool*3.14159265/180.0;
        next_positions[6] = -real_tool*3.14159265/180.0;
    }

    // pcontroller->SetDesired(next_positions); // This function "resets" physics
//    probot->SetJointValues(next_positions);  // More compatible with physics??
      probot->SetJointValues(next_positions);  // More compatible with physics??
    
    penv->StepSimulation(THREAD_RATE/1000.0);  // Must be given in seconds
    if(cameraFound) {
        psensorbase->GetSensorData(pcamerasensordata);
        //std::vector<uint8_t> currentFrame = pcamerasensordata->vimagedata;
        // printf("Vector size: %d",currentFrame.size()); // = 480 * 640 * 3 = 921600;
        yarp::sig::ImageOf<yarp::sig::PixelRgb>& i_imagen = p_imagen.prepare(); 
        i_imagen.resize(640,480);  // Tamaño de la pantalla (640,480)
        yarp::sig::PixelRgb p;
        for (int i_x = 0; i_x < i_imagen.width(); ++i_x) {
            for (int i_y = 0; i_y < i_imagen.height(); ++i_y) {
                p.r = pcamerasensordata->vimagedata[3*(i_x+(i_y*i_imagen.width()))];
                p.g = pcamerasensordata->vimagedata[1+3*(i_x+(i_y*i_imagen.width()))];
                p.b = pcamerasensordata->vimagedata[2+3*(i_x+(i_y*i_imagen.width()))];
                i_imagen.safePixel(i_x,i_y) = p;
            }
        }
        p_imagen.write();
    }
  }

 private:
  // General Joint Motion Controller parameters //
  int modePosVel;
  int joint_status[NUM_MOTORS];
  double real_degrees[NUM_MOTORS];
  double target_degrees[NUM_MOTORS];
  double joint_vel[NUM_MOTORS];
  double gvels[NUM_MOTORS];
  // Tool-related
//  bool tool_status;
//  double target_tool;
  double real_tool;
  toolPort theToolPort;  
  // Rave-specific parameters //
  bool toolFound;
  bool cameraFound;
  EnvironmentBasePtr penv;
  PhysicsEngineBasePtr pe;
  RobotBasePtr probot;
  ControllerBasePtr pcontrol;
  SensorBasePtr psensorbase;
  boost::shared_ptr<SensorBase::CameraSensorData> pcamerasensordata;
  std::vector<KinBodyPtr> bodies;
  std::vector<KinBody::JointPtr> mismotores;
  // YARP
  BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > p_imagen;
};


