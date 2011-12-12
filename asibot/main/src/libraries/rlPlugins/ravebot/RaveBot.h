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

#define DEFAULT_ENV_NAME "/share/models/asibot_cocina_entero.env.xml"  // on $YARPMODS_DIR

using namespace std;

using namespace yarp::os;
using namespace yarp::dev;

using namespace OpenRAVE;

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername);

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
 * installation steps refer to http://sourceforge.net/apps/trac/roboticslab/wiki/WikiYarpmods
 *
 */
class RaveBot : public DeviceDriver, public RateThread, public IPositionControl, public IVelocityControl, public IEncoders {
 public:

  // Set the Thread Rate in the class constructor
  RaveBot() : RateThread(THREAD_RATE) {}  // In ms

 // ------------------ IPositionControl Related -------------------------------
  /**
   * Get the number of controlled axes. This command asks the number of controlled
   * axes for the current physical interface.
   * @param ax pointer to storage
   * @return true/false.
   */
  virtual bool getAxes(int *ax) {
    *ax = NUM_MOTORS;
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
    printf("RaveBot: setPositionMode().\n");
    stop();
    modePosVel = 0;
    return true;
  }

  /** Set new reference point for a single axis.
   * @param j joint number
   * @param ref specifies the new ref point
   * @return true/false on success/failure
   */
  virtual bool positionMove(int j, double ref) {
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::positionMove(%d,%f) [begin]\n",j,ref);
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
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::positionMove() [begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    for(int motor=0;motor<NUM_MOTORS;motor++)
      if (fabs(refs[motor]-real_degrees[motor])>max_dist)
        max_dist = fabs(refs[motor]-real_degrees[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0;motor<NUM_MOTORS;motor++) {
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
    //Make sure we are in Position Mode
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::relativeMove(%d,%f) [begin]\n",j,delta);
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
    if(modePosVel!=0) {
        printf("RaveBot: Not in position mode.\n");
        return false;
    } else printf("RaveBot::relativeMove() [begin]\n");
    // Find out the maximum angle to move
    double max_dist = 0;
    for(int motor=0;motor<5;motor++)
      if (fabs(deltas[motor])>max_dist)
        max_dist = fabs(deltas[motor]);
    // Set all the private parameters of the Rave class that correspond to this kind of movement!
    for(int motor=0; motor<5; motor++) {
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
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
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
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
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
    for (unsigned int i=0; i<NUM_MOTORS; i++)
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
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      joint_vel[i]=0.0;
      joint_status[i]=-1;
      theToolPort.status=0;
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
    for (unsigned int i=0; i<NUM_MOTORS; i++)
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
    for (unsigned int i=0; i<NUM_MOTORS; i++)
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
    for (unsigned int i=0; i<NUM_MOTORS; i++)
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
    for (unsigned int i=0; i<NUM_MOTORS; i++)
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


 // ------------------- DeviceDriver Related ------------------------------------
  virtual bool open(Searchable& config) {

    if(config.check("help")) {
       printf("\n");
       printf("Option: --help  -------> This help\n");
       printf("Option: --env [env]  --> [env] env name (abs, or rel to YARPMODS_DIR, defaults to %s)\n",DEFAULT_ENV_NAME);
       printf("\n");
       exit(1);
    }

    // Create the name of the scene to load, three options:
    //   1) User gives us nothing -> we use $YARPMODS/default
    //   2) User gives us abs -> we use "env"
    //   2) User gives us rel -> we use $YARPMODS/"env"
    char *c_yarpmodsdir;
    c_yarpmodsdir = getenv("YARPMODS_DIR");
    if(c_yarpmodsdir) {
      printf("$YARPMODS_DIR is set to: %s\n",c_yarpmodsdir);
    } else {
      printf("[warning] $YARPMODS_DIR is not set, should look like ~/yarpmods/build ...\n\n");
      if(!config.check("env"))
        printf("[error] no --env [env] parameter found either, leaving...\n");
        exit(1);
    }
    // If it reaches up to here, we at least have one of them.

    // Initialize OpenRAVE-core
    RaveInitialize(true);  // Start openrave core
    penv = RaveCreateEnvironment();  // Create the main OpenRAVE environment, set the EnvironmentBasePtr
    penv->SetDebugLevel(Level_Debug);  // Relatively new function
    penv->StopSimulation();  // NEEDED??
    boost::thread thviewer(boost::bind(SetViewer,penv,"qtcoin"));
    Time::delay(0.4); // wait for the viewer to init, in [s]

    // Actually load the scene
    if(config.check("env")){
        ConstString envAsAbs = config.find("env").toString();
        printf("Checking for [env] as absolute: ");
        if(!penv->Load(envAsAbs.c_str())){
            printf("not found...\n\n");
            if(c_yarpmodsdir) {
                std::string envAsRel(c_yarpmodsdir);
                envAsRel.append(envAsAbs.c_str());
                printf("Checking for [env] as relative: ");
                if(!penv->Load(envAsRel.c_str())) {
                    printf("not found...\n\n");
                    std::string myEnvironment(c_yarpmodsdir);
                    myEnvironment.append(DEFAULT_ENV_NAME);
                    printf("Checking for default environment: ");
                    if(!penv->Load(myEnvironment)){
                        printf("not found... leaving!\n\n");
                        exit(-1);
                    } printf("found!\n");
                } else printf("found!\n");
            }
        } else printf("found!\n");
    } else {
        std::string myEnvironment(c_yarpmodsdir);
        myEnvironment.append(DEFAULT_ENV_NAME);
        printf("No [env], checking for default environment: ");
        if(!penv->Load(myEnvironment)){
            printf("not found... leaving!\n\n");
            exit(-1);
        } else printf("found!\n");
    }

    // Initialize private parameters
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      joint_status[i]=0;
      real_degrees[i]=0.0;
      joint_vel[i]=0.0;
      target_degrees[i]=0.0;
      gvels[i]=100.0;
    }
    modePosVel = 0;  // 0 = Pos; 1 = Vel.

    // NEW: Attach a physics engine
    penv->SetPhysicsEngine(RaveCreatePhysicsEngine(penv,"ode"));
    penv->GetPhysicsEngine()->SetGravity(Vector(0,0,-9.8));

    // OLD: Set the transform matrix for the camera view
    RaveTransformMatrix<float> M;
//    RaveVector<float> rotquad(0.505073f, 0.268078f, 0.395983f, 0.718493f);
    RaveVector<float> trans(4.0, 2.0, 3.0);
    M.trans = trans;
//    M.rotfromquat (rotquad);  // Doesn't work anymore...
    RaveTransform<float> Tcamera(M);

    //-- Get the robot
    std::vector<RobotBasePtr> robots;
    penv->GetRobots(robots);
    //-- Robot 0
    probot = robots.at(0);  // which is a RobotBasePtr
    printf("Robot: %s.\n", probot->GetName().c_str());

    if(!UNSTABLE) {
        std::vector<KinBody::LinkPtr> links = probot->GetLinks();
        KinBody::LinkPtr plink = links.at(0);
        plink->SetStatic(true);
    }

    cameraFound = false;
    unsigned int robotIter = 0;
    while ( (robotIter < robots.size()) && (!cameraFound) ) {
        std::vector<RobotBase::AttachedSensorPtr> sensors = robots.at(robotIter)->GetAttachedSensors();
        if(sensors.size() > 0) {
            printf("Sensors found on robot %d (%s).\n",robotIter,robots.at(robotIter)->GetName().c_str());
            psensorbase = sensors.at(0)->GetSensor();
            if(psensorbase != NULL) {
                printf("Good sensorbase...\n");
                std::string tipo = psensorbase->GetDescription();
                printf("%s\n",tipo.c_str());
                tipo = psensorbase->GetName();
                printf("%s\n",tipo.c_str());
                // Get some camera parameter info
                boost::shared_ptr<SensorBase::CameraGeomData> pcamerageomdata = boost::dynamic_pointer_cast<SensorBase::CameraGeomData>(psensorbase->GetSensorGeometry(SensorBase::ST_Camera));
                int imgw = pcamerageomdata->width;
                int imgh = pcamerageomdata->height;
                printf("Camera width: %d, height: %d.\n",imgw,imgh);
                // Get a pointer to access the camera data stream
                pcamerasensordata = boost::dynamic_pointer_cast<SensorBase::CameraSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Camera));
                // Activate the camera
                stringstream sin, sout;
                sin << "power 1";
                if (psensorbase->SendCommand(sout,sin))
                    cout << "Sending power camera on command success, response: " << sout.str() << endl;
                else printf("[error] Could not power camera on.\n");
                p_imagen.open("/ravebot/img:o");
                cameraFound = true;
            } else printf("[warning] Bad sensorbase, may break in future because of this.\n");
        } else printf("No sensors found on robot %d (%s).\n",robotIter,robots.at(robotIter)->GetName().c_str());
        robotIter++;
    }

    pcontrol = RaveCreateController(penv,"idealcontroller");
    // Create the controllers, make sure to lock environment! (prevents changes)
    {
      EnvironmentMutex::scoped_lock lock(penv->GetMutex());
      std::vector<int> dofindices(probot->GetDOF());
      for(int i = 0; i < probot->GetDOF(); ++i) {
        dofindices[i] = i;
//        printf("HERE: %d\n",i);
      }
      probot->SetController(pcontrol,dofindices,1); // control everything
    }

    //-- KinBody
    penv->GetBodies(bodies);  // std::vector<KinBodyPtr> bodies;
    //-- Joints
    mismotores = bodies[0]->GetJoints();  // KinBody::JointPtr
    if(mismotores.size()==7) {
        Value toolportvalue = config.check("toolport","/ravebot/tool:i","checking if given an alternate tool port name");
        theToolPort.open(toolportvalue.asString());
        toolFound = true;
        theToolPort.useCallback();
    } else {
        toolFound = false;
        printf("No tool attatched.\n");
    }

    // Start the RateThread
    this->start();
    
    return true;
  }


  virtual bool close() {
//    pthviewer->join(); // wait for the viewer thread to exit
//    printf("RaveBot: close()\n");
    penv->Destroy(); // destroy
    return true;
  }


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

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername)
{
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);

    // attach it to the environment:
    penv->AttachViewer(viewer);

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true;
    viewer->main(showgui);
}

