// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RAVEBOT_H__
#define __RAVEBOT_H__

#include <yarp/os/all.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/all.h>

#include <openrave-core.h>

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "WorldRpcResponder.h"
#include "MobileRpcResponder.h"
#include "ExtraCallbackPort.h"

#define DEFAULT_NUM_MOTORS 5

#define DEFAULT_ENV "models/asibot_kitchen.env.xml"
#define DEFAULT_EXTRA_ROBOT "none"
#define DEFAULT_EXTERN_OBJ "none"
#define DEFAULT_GEN_ENC_RAW_EXPOSED 0.0174532925199433  // Ratio, 0.0174532925199433 is pi/180 (raw/exp)<->(rad/deg)
#define DEFAULT_GEN_INIT_POS 0  // Exposed.
#define DEFAULT_GEN_JOINT_TOL 0.25  // Exposed.
#define DEFAULT_GEN_MAX_LIMIT 360.0  // Exposed.
#define DEFAULT_GEN_MIN_LIMIT -360.0  // Exposed.
#define DEFAULT_GEN_REF_SPEED 7.5  // Exposed.
#define DEFAULT_GEN_VEL_RAW_EXPOSED 0.0174532925199433  // Ratio, 0.0174532925199433 is pi/180 (raw/exp)<->(rad/deg)
#define DEFAULT_JMC_MS 20  // [ms]
#define DEFAULT_JMC_MS_ACC 1  // multiplier
#define DEFAULT_MODE_POS_VEL VOCAB_POSITION_MODE
#define DEFAULT_PHYSICS "none"
#define DEFAULT_VIEWER 1

using namespace std;

using namespace yarp::os;
using namespace yarp::dev;

using namespace OpenRAVE;

/**
 * @ingroup RlPlugins
 * \defgroup RaveBot
 *
 * \image html ravebot.png
 *
 * The \ref RaveBot library is composed by a single class, RaveBot.
 *
 * <b>Installation</b>
 *
 * The plugin is compiled when ENABLE_RlPlugins_ravebot is activated (not default). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 */

/**
 *
 * @ingroup RaveBot
 * The RaveBot class creates an instance of OpenRAVE-core (qtcoin viewer included) and implements the
 * YARP_dev IPositionControl, IVelocityControl and IEncoders interface class member functions.
 * It additionally publishes the stream of images from a camera set on robot[0] (default image
 * port name: /ravebot/img:o). It is used by <a class="el" href="group__testRaveBot.html">testRaveBot</a>
 * and <a class="el" href="group__cartesianServer.html">cartesianServer</a>.
 *
 */
class RaveBot : public DeviceDriver, public RateThread, public IPositionControl, public IVelocityControl, public IEncodersTimed, public IControlLimits, public IControlMode {
 public:

  // Set the Thread Rate in the class constructor
  RaveBot() : RateThread(DEFAULT_JMC_MS) {}  // In ms

// ------- IPositionControl declarations. Implementation in IPositionImpl.cpp -------

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
     * @note kept for backwards compatibility with YARP <2.3.65,
     * use \ref setPositionMode(int) instead
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

//  ---------- IEncoder Declarations. Implementation in IEncoderImpl.cpp ----------

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

    /**
     * Read the instantaneous acceleration of all axes.
     * \param encs pointer to the array that will contain the output
     * \param time pointer to the array that will contain individual timestamps
     * \return true if all goes well, false if anything bad happens.
     */
    virtual bool getEncodersTimed(double *encs, double *time);

    /**
     * Read the instantaneous acceleration of all axes.
     * \param j axis index
     * \param encs encoder value (pointer to)
     * \param time corresponding timestamp (pointer to)
     * \return true if all goes well, false if anything bad happens.
     */
    virtual bool getEncoderTimed(int j, double *encs, double *time);


//  --------- IVelocityControl Declarations. Implementation in IVelocityImpl.cpp ---------

    /**
     * Set velocity mode. This command
     * is required by control boards implementing different
     * control methods (e.g. velocity/torque), in some cases
     * it can be left empty.
     * @return true/false on success failure
     * @note kept for backwards compatibility with YARP <2.3.65,
     * use \ref setVelocityMode(int) instead
     */
    virtual bool setVelocityMode();

    /**
     * Start motion at a given speed, single joint.
     * @param j joint number
     * @param sp speed value
     * @return bool/false upone success/failure
     */
    virtual bool velocityMove(int j, double sp);

    /**
     * Start motion at a given speed, multiple joints.
     * @param sp pointer to the array containing the new speed values
     * @return true/false upon success/failure
     */
    virtual bool velocityMove(const double *sp);

//  --------- IControlLimits Declarations. Implementation in IControlLimitsImpl.cpp ---------

    /**
     * Set the software limits for a particular axis, the behavior of the
     * control card when these limits are exceeded, depends on the implementation.
     * @param axis joint number (why am I telling you this)
     * @param min the value of the lower limit
     * @param max the value of the upper limit
     * @return true or false on success or failure
     */
    virtual bool setLimits(int axis, double min, double max);
    
    /* Get the software limits for a particular axis.
     * @param axis joint number (again... why am I telling you this)
     * @param pointer to store the value of the lower limit
     * @param pointer to store the value of the upper limit
     * @return true if everything goes fine, false otherwise.
     */
    virtual bool getLimits(int axis, double *min, double *max);

// -------- IControlMode declarations. Implementation in IControlModeImpl.cpp --------

    /**
     * Set position mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setPositionMode(int j);

    /**
     * Set velocity mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setVelocityMode(int j);

    /**
     * Set torque mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setTorqueMode(int j);

    /**
     * Set impedance position mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setImpedancePositionMode(int j);

    /**
     * Set impedance velocity mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setImpedanceVelocityMode(int j);

    /**
     * Set open loop mode, single axis.
     * @param j joint number
     * @return: true/false success failure.
     */
    virtual bool setOpenLoopMode(int j);

    /**
     * Get the current control mode.
     * @param j joint number
     * @param mode a vocab of the current control mode for joint j.
     * @return: true/false success failure.
     */
    virtual bool getControlMode(int j, int *mode);

    /**
     * Get the current control mode (multiple joints).
     * @param modes a vector containing vocabs for the current control modes of the joints.
     * @return: true/false success failure.
     */
    virtual bool getControlModes(int *modes);

// -------- DeviceDriver declarations. Implementation in IDeviceImpl.cpp --------

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

// -------- RateThread declarations. Implementation in RateThreadImpl.cpp --------

    /**
     * Initialization method. The thread executes this function
     * when it starts and before "run". This is a good place to 
     * perform initialization tasks that need to be done by the 
     * thread itself (device drivers initialization, memory 
     * allocation etc). If the function returns false the thread 
     * quits and never calls "run". The return value of threadInit()
     * is notified to the class and passed as a parameter 
     * to afterStart(). Note that afterStart() is called by the 
     * same thread that is executing the "start" method.
     */
    bool threadInit();

    /**
     * Loop function. This is the thread itself.
     */
    void run();

// ----- Shared Area Funcion declarations. Implementation in SharedArea.cpp -----
    void setEncRaw(const int Index, const double Position);
    double getEncRaw(const int Index);
    double getEncExposed(const int Index);

// ------------------------------- Private -------------------------------------
  private:
    int viewer;
    // General Joint Motion Controller parameters //
    unsigned int numMotors;
    //
    std::vector<int> vModePosVel;
    double lastTime;
    Semaphore encRawMutex;  // SharedArea
    std::vector<double> encRaw;
    std::vector<double> encRawExposed;  // For conversion.
    std::vector<int> jointStatus;
    std::vector<double> initPos;  // Exposed.
    std::vector<double> jointTol;  // Exposed.
    std::vector<double> maxLimit;  // Exposed.
    std::vector<double> minLimit;  // Exposed.
    std::vector<double> refAcc;  // Exposed.
    std::vector<double> refSpeed;  // Exposed.
    std::vector<double> targetExposed;  // Exposed.
    std::vector<double> velRawExposed;  // For conversion.
    std::vector<double> velRaw;
    double jmcMs, jmcMsAcc;
    // Rave-specific parameters //
    EnvironmentBasePtr penv;
    PhysicsEngineBasePtr pe;
    RobotBasePtr probot;
    RobotBase::ManipulatorPtr probotManip;
    int drawCounter;
    int drawnElems;
    //
    RobotBasePtr pmobile;
    ModuleBasePtr pbasemanip;
    ControllerBasePtr pcontrol;
    //
    RobotBasePtr pndof;
    //
    std::vector<KinBodyPtr> bodies;
    std::vector<KinBody::JointPtr> mismotores;
    boost::thread_group orThreads;
    //
    std::vector< SensorBasePtr > pcamerasensorbase;
    std::vector< SensorBasePtr > plasersensorbase;
    std::vector< SensorBasePtr > pforce6dsensorbase;
    std::vector< boost::shared_ptr<SensorBase::CameraSensorData> > pcamerasensordata;
    std::vector< boost::shared_ptr<SensorBase::LaserSensorData> >  plasersensordata;
    std::vector< boost::shared_ptr<SensorBase::Force6DSensorData> >  pforce6dsensordata;
    std::vector<int> cameraWidth;
    std::vector<int> cameraHeight;
    // YARP_sig (simulated camera image publishing)
    std::vector< BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* > p_imagen;
    std::vector< BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* > p_depth;
    std::vector< BufferedPort<yarp::os::Bottle>* > p_force6d;
    // world rpc server
    RpcServer worldRpcServer;
    WorldRpcResponder worldRpcResponder;
    // mobile rpc server
    RpcServer mobileRpcServer;
    MobileRpcResponder mobileRpcResponder;
    // pndof
    ExtraCallbackPort extraCallbackPort;
};

#endif

