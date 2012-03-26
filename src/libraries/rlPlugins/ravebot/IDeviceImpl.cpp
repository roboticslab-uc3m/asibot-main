
// -----------------------------------------------------------------------------

#include "RaveBot.h"

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername);

// ------------------- DeviceDriver Related ------------------------------------

bool RaveBot::open(Searchable& config) {

    modePosVel = 0;  // 0 = Pos; 1 = Vel;
    for (unsigned int i=0; i<NUM_MOTORS; i++) {
      joint_status[i]=0;
      real_degrees[i]=0.0;
      joint_vel[i]=0.0;
      target_degrees[i]=0.0;
      refAcc[i]=1.0;
    }

    msJoint = DEFAULT_MSJOINT;
    ConstString env = DEFAULT_ENV;
    for (unsigned int i=0; i<NUM_MOTORS; i++) refSpeed[i]=DEFAULT_REFSPEED;
    minLimit[0] = DEFAULT_MINLIMIT0;
    minLimit[1] = DEFAULT_MINLIMIT1;
    minLimit[2] = DEFAULT_MINLIMIT2;
    minLimit[3] = DEFAULT_MINLIMIT3;
    minLimit[4] = DEFAULT_MINLIMIT4;
    maxLimit[0] = DEFAULT_MAXLIMIT0;
    maxLimit[1] = DEFAULT_MAXLIMIT1;
    maxLimit[2] = DEFAULT_MAXLIMIT2;
    maxLimit[3] = DEFAULT_MAXLIMIT3;
    maxLimit[4] = DEFAULT_MAXLIMIT4;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("RaveBot options:\n");
        printf("\t--help (this help)\n");
        printf("\t--msJoint [ms] (rate of joint control thread, default: \"%f\")\n",msJoint);
        printf("\t--env [xml] (env in abs or rel to \"$ASIBOT_ROOT/app/ravebot/models\", default: \"%s\")\n",env.c_str());
        printf("\t--refSpeed [deg/s] (default: %f)\n",refSpeed[0]);
    }

    char *asibot_root;
    asibot_root = getenv("ASIBOT_ROOT");
    if(!asibot_root) printf("[warning] $ASIBOT_ROOT is not set.\n");

    if (config.check("env")) env = config.find("env").asString();
    if (config.check("msJoint")) msJoint = config.find("msJoint").asDouble();
    if (config.check("refSpeed")) {
        for (unsigned int i=0; i<NUM_MOTORS; i++) refSpeed[i] = config.find("refSpeed").asDouble();
    }
    printf("RaveBot using env: %s.\n",env.c_str());
    printf("RaveBot using msJoint: %f, refSpeed: %f.\n",msJoint,refSpeed[0]);

    // If it reaches up to here, we at least have one of them.
    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        exit(1);
    }

    // Initialize OpenRAVE-core
    RaveInitialize(true);  // Start openrave core
    penv = RaveCreateEnvironment();  // Create the main OpenRAVE environment, set the EnvironmentBasePtr
    penv->SetDebugLevel(Level_Debug);  // Relatively new function
    penv->StopSimulation();  // NEEDED??
    boost::thread thviewer(boost::bind(SetViewer,penv,"qtcoin"));
    Time::delay(0.4); // wait for the viewer to init, in [s]


    // Actually load the scene
    ConstString envAbsFile(asibot_root);
    envAbsFile += "/app/ravebot/models/";
    envAbsFile += env;

    if(!penv->Load(envAbsFile.c_str())) {
        printf("[warning] RaveBot could not load %s environment.\n",envAbsFile.c_str());
        if (!penv->Load(env.c_str())) {
            printf("[error] RaveBot could not load %s environment either.\n",env.c_str());
            return false;
        }
    }
    printf("[success] RaveBot loaded environment.\n");

    // NEW: Attach a physics engine
    penv->SetPhysicsEngine(RaveCreatePhysicsEngine(penv,"ode"));
    penv->GetPhysicsEngine()->SetGravity(Vector(0,0,-9.8));

    // OLD: Set the transform matrix for the camera view
//    RaveTransformMatrix<float> M;
//    RaveVector<float> rotquad(0.505073f, 0.268078f, 0.395983f, 0.718493f);
//    RaveVector<float> trans(4.0, 2.0, 3.0);
//    M.trans = trans;
//    M.rotfromquat (rotquad);  // Doesn't work anymore...
//    RaveTransform<float> Tcamera(M);

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
    this->setRate(msJoint);
    this->start();
    
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::close() {
//    pthviewer->join(); // wait for the viewer thread to exit
//    printf("RaveBot: close()\n");
    penv->Destroy(); // destroy
    return true;
}

// -----------------------------------------------------------------------------

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername)
{
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);

    // attach it to the environment:
    penv->AttachViewer(viewer);

    RaveVector<float> _cameraTrans(2.0,3.2,2.8);
    RaveVector<float> _cameraAxis(0.092729,0.413007,0.905995);
    float _cameraAngle = 2.863747;
    RaveVector<float> _cameraQuat = quatFromAxisAngle(_cameraAxis,_cameraAngle);
    RaveTransform<float> Tcamera(_cameraQuat,_cameraTrans);
    viewer->SetCamera(Tcamera,0.785398);

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true;
    viewer->main(showgui);
}

