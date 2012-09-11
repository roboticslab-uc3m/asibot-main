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

#define DEFAULT_NUM_MOTORS 5

#define DEFAULT_ENV "asibot_cocina_entero.env.xml"
#define DEFAULT_GEN_ENC_RAW_EXPOSED 1.0  // Ratio.
#define DEFAULT_GEN_JOINT_TOL 0.25  // Exposed.
#define DEFAULT_GEN_MAX_LIMIT 360.0  // Exposed.
#define DEFAULT_GEN_MIN_LIMIT -360.0  // Exposed.
#define DEFAULT_GEN_REF_SPEED 7.5  // Exposed.
#define DEFAULT_GEN_VEL_RAW_EXPOSED 1.0  // Ratio.
#define DEFAULT_JMC_MS 20  // [ms]
#define DEFAULT_PHYSICS "ode"

#define TOOL_SPEED_ADJ 0.08  // Speed adjustment for tool movement (only simulation).
#define UNSTABLE false

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
 * @ingroup rlPlugins
 * \defgroup RaveBot
 *
 * \image html ravebot.png
 *
 * The \ref RaveBot library is composed by a single class, RaveBot.
 *
 * <b>Installation</b>
 *
 * The plugin is compiled when ENABLE_rlPlugins_ravebot is activated (not default). For further
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
class RaveBot : public DeviceDriver, public RateThread, public IPositionControl, public IVelocityControl, public IEncoders, public IControlLimits {
 public:

  // Set the Thread Rate in the class constructor
  RaveBot() : RateThread(DEFAULT_JMC_MS) {}  // In ms

// -- Helper Funcion declarations. Implementation in HelperFuncs.cpp--

    /**
    * Simple function to pass from degrees to radians. 
    * @param inDeg angle value in degrees.
    * @return angle value in radians.
    */
    double toRad(const double _inDeg);

    /**
    * Simple function to pass from radians to degrees.
    * @param _inRad angle value in radians.
    * @return angle value in degrees.
    */
    double toDeg(const double _inRad);

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

//  --------- IVelocityControl Declarations. Implementation in IVelocityImpl.cpp ---------

    /**
     * Set velocity mode. This command
     * is required by control boards implementing different
     * control methods (e.g. velocity/torque), in some cases
     * it can be left empty.
     * @return true/false on success failure
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
    // General Joint Motion Controller parameters //
    unsigned int numMotors;
    //
    int modePosVel;
    double lastTime;
    Semaphore encRawMutex;  // SharedArea
    std::vector<double> encRaw;
    std::vector<double> encRawExposed;  // For conversion.
    std::vector<int> jointStatus;
    std::vector<double> jointTol;  // Exposed.
    std::vector<double> maxLimit;  // Exposed.
    std::vector<double> minLimit;  // Exposed.
    std::vector<double> refAcc;  // Exposed.
    std::vector<double> refSpeed;  // Exposed.
    std::vector<double> targetExposed;  // Exposed.
    std::vector<double> velRawExposed;  // For conversion.
    std::vector<double> velRaw;
    double jmcMs;
    // Tool-related
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
    // YARP_sig (simulated camera image publishing)
    BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > p_imagen;
};

#endif

