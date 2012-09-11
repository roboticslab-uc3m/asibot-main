
// -----------------------------------------------------------------------------

#include "RaveBot.h"

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername);

// ------------------- DeviceDriver Related ------------------------------------

bool RaveBot::open(Searchable& config) {

    numMotors = DEFAULT_NUM_MOTORS;

    ConstString env = DEFAULT_ENV;
    double genJointTol = DEFAULT_GEN_JOINT_TOL;
    double genMinLimit = DEFAULT_GEN_MIN_LIMIT;
    double genMaxLimit = DEFAULT_GEN_MAX_LIMIT;
    double genRefSpeed = DEFAULT_GEN_REF_SPEED;
    jmcMs = DEFAULT_JMC_MS;
    ConstString physics = DEFAULT_PHYSICS;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("RaveBot options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--numMotors [int] (number of motors to control, default: \"%d\")\n",numMotors);

        printf("\t--env [xml] (env in abs or rel to \"$ASIBOT_ROOT/app/ravebot/models\", default: \"%s\")\n",env.c_str());
        printf("\t--genJointTol [units] (default: \"%f\")\n",genJointTol);
        printf("\t--genMaxLimit [units] (default: \"%f\")\n",genMaxLimit);
        printf("\t--genMinLimit [units] (default: \"%f\")\n",genMinLimit);
        printf("\t--genRefSpeed [units/s] (default: \"%f\")\n",genRefSpeed);
        printf("\t--jmcMs [ms] (rate of Joint Motion Controller thread, default: \"%f\")\n",jmcMs);
        printf("\t--physics [type] (type of physics, default: \"%s\")\n",physics.c_str());
    }

    char *asibot_root;
    asibot_root = getenv("ASIBOT_ROOT");
    if(!asibot_root) printf("[warning] $ASIBOT_ROOT is not set.\n");

    if (config.check("numMotors")) numMotors = config.find("numMotors").asDouble();
    if (config.check("env")) env = config.find("env").asString();
    printf("RaveBot using numMotors: %d, env: %s.\n",numMotors,env.c_str());

    if (config.check("genJointTol")) genJointTol = config.find("genJointTol").asDouble();
    if (config.check("genMaxLimit")) genMaxLimit = config.find("genMaxLimit").asDouble();
    if (config.check("genMinLimit")) genMinLimit = config.find("genMinLimit").asDouble();
    if (config.check("genRefSpeed")) genRefSpeed = config.find("genRefSpeed").asDouble();
    if (config.check("jmcMs")) jmcMs = config.find("jmcMs").asDouble();
    if (config.check("physics")) physics = config.find("physics").asString();
    printf("RaveBot using genJointTol: %f, genMaxLimit: %f, genMinLimit: %f, genRefSpeed: %f.\n",
        genJointTol,genMaxLimit,genMinLimit,genRefSpeed);
    printf("RaveBot using jmcMs: %f, physics: %s.\n",jmcMs,physics.c_str());

    Bottle* jointTols;
    if (config.check("jointTols")) {
        jointTols = config.find("jointTols").asList();
        printf("RaveBot using individual jointTols: %s\n",jointTols->toString().c_str());
        if(jointTols->size() != int(numMotors)) printf("[warning] jointTols->size() != numMotors\n");
    } else {
        jointTols = 0;
        printf("RaveBot not using individual jointTols, defaulting to genJointTols.\n");
    }
    Bottle* maxLimits;
    if (config.check("maxLimits")) {
        maxLimits = config.find("maxLimits").asList();
        printf("RaveBot using individual maxLimits: %s\n",maxLimits->toString().c_str());
        if(maxLimits->size() != int(numMotors)) printf("[warning] maxLimits->size() != numMotors\n");
    } else {
        maxLimits = 0;
        printf("RaveBot not using individual maxLimits, defaulting to genMaxLimit.\n");
    }
    Bottle* minLimits;
    if (config.check("minLimits")) {
        minLimits = config.find("minLimits").asList();
        printf("RaveBot using individual minLimits: %s\n",minLimits->toString().c_str());
        if(minLimits->size() != int(numMotors)) printf("[warning] minLimits->size() != numMotors\n");
    } else {
        minLimits = 0;
        printf("RaveBot not using individual minLimits, defaulting to genMinLimit.\n");
    }
    Bottle* refSpeeds;
    if (config.check("refSpeeds")) {
        refSpeeds = config.find("refSpeeds").asList();
        printf("RaveBot using individual refSpeeds: %s\n",refSpeeds->toString().c_str());
        if(refSpeeds->size() != int(numMotors)) printf("[warning] refSpeeds->size() != numMotors\n");
    } else {
        refSpeeds = 0;
        printf("RaveBot not using individual refSpeeds, defaulting to genRefSpeed.\n");
    }

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        exit(1);
    }

    modePosVel = 0;  // 0 = Pos; 1 = Vel;

    jointStatus.resize(numMotors);
    refSpeed.resize(numMotors);
    minLimit.resize(numMotors);
    maxLimit.resize(numMotors);
    encRaw.resize(numMotors);
    velRaw.resize(numMotors);
    targetExposed.resize(numMotors);
    refAcc.resize(numMotors);
    jointTol.resize(numMotors);
    for (unsigned int i=0; i<numMotors; i++) {
        jointStatus[i]=0;
        if(!refSpeeds) refSpeed[i]=genRefSpeed;
        else refSpeed[i]=refSpeeds->get(i).asDouble();
        if(!minLimits) minLimit[i]=genMinLimit;
        else minLimit[i]=minLimits->get(i).asDouble();
        if(!maxLimits) maxLimit[i]=genMaxLimit;
        else maxLimit[i]=maxLimits->get(i).asDouble(); 
        if(!jointTols) jointTol[i]=genJointTol;
        else jointTol[i]=jointTols->get(i).asDouble(); 
        encRaw[i]=0.0;
        velRaw[i]=0.0;
        targetExposed[i]=0.0;
        refAcc[i]=1.0;
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
    if(physics!="none"){
        penv->SetPhysicsEngine(RaveCreatePhysicsEngine(penv,"ode"));
        penv->GetPhysicsEngine()->SetGravity(Vector(0,0,-9.8));
    }

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
    if(mismotores.size()>int(numMotors)) { // Big hack
        Value toolportvalue = config.check("toolport","/ravebot/tool:i","checking if given an alternate tool port name");
        theToolPort.open(toolportvalue.asString());
        toolFound = true;
        theToolPort.useCallback();
    } else {
        toolFound = false;
        printf("No tool attatched.\n");
    }

    // Start the RateThread
    this->setRate(jmcMs);
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
//    viewer->SetCamera(Tcamera,0.785398);

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true;
    viewer->main(showgui);
}

