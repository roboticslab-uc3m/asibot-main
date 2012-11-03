
// -----------------------------------------------------------------------------

#include "RaveBot.h"

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername);

// ------------------- DeviceDriver Related ------------------------------------

bool RaveBot::open(Searchable& config) {

    numMotors = DEFAULT_NUM_MOTORS;
    jmcMs = DEFAULT_JMC_MS;

    ConstString env = DEFAULT_ENV;
    double genJointTol = DEFAULT_GEN_JOINT_TOL;
    double genMaxLimit = DEFAULT_GEN_MAX_LIMIT;
    double genMinLimit = DEFAULT_GEN_MIN_LIMIT;
    double genRefSpeed = DEFAULT_GEN_REF_SPEED;
    double genEncRawExposed = DEFAULT_GEN_ENC_RAW_EXPOSED;
    double genVelRawExposed = DEFAULT_GEN_VEL_RAW_EXPOSED;
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
        printf("\t--genEncRawExposed (encoder [raw / exposed] ratio, default: \"%f\")\n",genEncRawExposed);
        printf("\t--genVelRawExposed (velocity [raw / exposed] ratio, default: \"%f\")\n",genVelRawExposed);
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
    if (config.check("genEncRawExposed")) genEncRawExposed = config.find("genEncRawExposed").asDouble();
    if (config.check("genVelRawExposed")) genVelRawExposed = config.find("genVelRawExposed").asDouble();    
    if (config.check("jmcMs")) jmcMs = config.find("jmcMs").asDouble();
    if (config.check("physics")) physics = config.find("physics").asString();
    printf("RaveBot using genJointTol: %f, genMaxLimit: %f, genMinLimit: %f, genRefSpeed: %f.\n",
        genJointTol,genMaxLimit,genMinLimit,genRefSpeed);
    printf("RaveBot using genEncRawExposed: %f, genVelRawExposed: %f.\n", genEncRawExposed, genVelRawExposed);
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
    Bottle* encRawExposeds;
    if (config.check("encRawExposeds")) {
        encRawExposeds = config.find("encRawExposeds").asList();
        printf("RaveBot using individual encRawExposeds: %s\n",encRawExposeds->toString().c_str());
        if(encRawExposeds->size() != int(numMotors)) printf("[warning] encRawExposeds->size() != numMotors\n");
    } else {
        encRawExposeds = 0;
        printf("RaveBot not using individual encRawExposeds, defaulting to genEncRawExposed.\n");
    }
    Bottle* velRawExposeds;
    if (config.check("velRawExposeds")) {
        velRawExposeds = config.find("velRawExposeds").asList();
        printf("RaveBot using individual velRawExposeds: %s\n",velRawExposeds->toString().c_str());
        if(velRawExposeds->size() != int(numMotors)) printf("[warning] velRawExposeds->size() != numMotors\n");
    } else {
        velRawExposeds = 0;
        printf("RaveBot not using individual velRawExposeds, defaulting to genVelRawExposed.\n");
    }

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        exit(1);
    }

    modePosVel = 0;  // 0 = Pos; 1 = Vel;

    encRawExposed.resize(numMotors);
    jointStatus.resize(numMotors);
    jointTol.resize(numMotors);
    maxLimit.resize(numMotors);
    minLimit.resize(numMotors);
    refSpeed.resize(numMotors);
    velRawExposed.resize(numMotors);
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
        if(!encRawExposeds) encRawExposed[i]=genEncRawExposed;
        else encRawExposed[i]=encRawExposeds->get(i).asDouble(); 
        if(!velRawExposeds) velRawExposed[i]=genVelRawExposed;
        else velRawExposed[i]=velRawExposeds->get(i).asDouble(); 
    }
    encRaw.resize(numMotors, 0.0);
    refAcc.resize(numMotors, 1.0);
    targetExposed.resize(numMotors, 0.0);
    velRaw.resize(numMotors, 0.0);

    // Initialize OpenRAVE-core
    RaveInitialize(true);  // Start openrave core
    penv = RaveCreateEnvironment();  // Create the main OpenRAVE environment, set the EnvironmentBasePtr
    penv->SetDebugLevel(Level_Debug);  // Relatively new function
    penv->StopSimulation();  // NEEDED??
    boost::thread thviewer(boost::bind(SetViewer,penv,"qtcoin"));
    orThreads.add_thread(&thviewer);
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
        penv->GetPhysicsEngine()->SetGravity(OpenRAVE::Vector(0,0,-9.8));
    }

    //-- Get the robot
    std::vector<RobotBasePtr> robots;
    penv->GetRobots(robots);
    //-- Robot 0
    probot = robots.at(0);  // which is a RobotBasePtr
    printf("Robot 0 name: %s.\n", probot->GetName().c_str());
    //-- Robot 1
    if(robots.size()>1) {
        pmobile = robots.at(1);  // which is a RobotBasePtr
    } else pmobile = RobotBasePtr();

    for ( unsigned int robotIter = 0; robotIter<robots.size(); robotIter++ ) {
        std::vector<RobotBase::AttachedSensorPtr> sensors = robots.at(robotIter)->GetAttachedSensors();
        printf("Sensors found on robot %d (%s): %d.\n",robotIter,robots.at(robotIter)->GetName().c_str(),sensors.size());
        for ( unsigned int sensorIter = 0; sensorIter<sensors.size(); sensorIter++ ) {
            SensorBasePtr psensorbase = sensors.at(sensorIter)->GetSensor();
            std::string tipo = psensorbase->GetName();
            printf("Sensor %d name: %s\n",sensorIter,tipo.c_str());
            // printf("Sensor %d description: %s\n",sensorIter,psensorbase->GetDescription().c_str());
            if (psensorbase->Supports(SensorBase::ST_Camera)) {
                printf("Sensor %d supports ST_Camera.\n", sensorIter);
                // Activate the camera
                psensorbase->Configure(SensorBase::CC_PowerOn);
                // Show the camera image in a separate window
                // pcamerasensorbase->Configure(SensorBase::CC_RenderDataOn);
                // Get some camera parameter info
                boost::shared_ptr<SensorBase::CameraGeomData> pcamerageomdata = boost::dynamic_pointer_cast<SensorBase::CameraGeomData>(psensorbase->GetSensorGeometry(SensorBase::ST_Camera));
                // printf("Camera width: %d, height: %d.\n",pcamerageomdata->width,pcamerageomdata->height);
                cameraWidth.push_back(pcamerageomdata->width);
                cameraHeight.push_back(pcamerageomdata->height);
                // Get a pointer to access the camera data stream
                pcamerasensordata.push_back(boost::dynamic_pointer_cast<SensorBase::CameraSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Camera)));
                pcamerasensorbase.push_back(psensorbase);  // "save"
                BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* tmpPort = new BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >;
                ConstString tmpName("/ravebot/");
                tmpName += psensorbase->GetName().c_str();
                tmpName += "/img:o";
                tmpPort->open(tmpName);
                p_imagen.push_back(tmpPort);
            } else if (psensorbase->Supports(SensorBase::ST_Laser)) {
                printf("Sensor %d supports ST_Laser.\n", sensorIter);
                // Activate the sensor
                psensorbase->Configure(SensorBase::CC_PowerOn);
                // Paint the rays in the OpenRAVE viewer
                psensorbase->Configure(SensorBase::CC_RenderDataOn);
                // Get some laser parameter info
                // boost::shared_ptr<SensorBase::LaserGeomData> plasergeomdata = boost::dynamic_pointer_cast<SensorBase::LaserGeomData>(psensorbase->GetSensorGeometry(SensorBase::ST_Laser));
                // printf("Laser resolution: %f   %f.\n",plasergeomdata->resolution[0],plasergeomdata->resolution[1]);
                // printf("Laser min_angle: %f   %f.\n",plasergeomdata->min_angle[0],plasergeomdata->min_angle[1]);
                // printf("Laser max_angle: %f   %f.\n",plasergeomdata->max_angle[0],plasergeomdata->max_angle[1]);
                // Get a pointer to access the laser data stream
                plasersensordata.push_back(boost::dynamic_pointer_cast<SensorBase::LaserSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Laser)));
                plasersensorbase.push_back(psensorbase);  // "save"
                BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelFloat> >* tmpPort = new BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelFloat> >;
                ConstString tmpName("/ravebot/");
                tmpName += psensorbase->GetName().c_str();
                tmpName += "/depth:o";
                tmpPort->open(tmpName);
                p_depth.push_back(tmpPort);
            } else if (psensorbase->Supports(SensorBase::ST_Force6D)) {
                printf("Sensor %d supports ST_Force6D.\n", sensorIter);
                // Activate the sensor
                psensorbase->Configure(SensorBase::CC_PowerOn);
                // Paint the rays in the OpenRAVE viewer
                psensorbase->Configure(SensorBase::CC_RenderDataOn);
                // Get a pointer to access the laser data stream
                pforce6dsensordata.push_back(boost::dynamic_pointer_cast<SensorBase::Force6DSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Force6D)));
                pforce6dsensorbase.push_back(psensorbase);  // "save"
                BufferedPort<Bottle>* tmpPort = new BufferedPort<Bottle>;
                ConstString tmpName("/ravebot/");
                tmpName += psensorbase->GetName().c_str();
                tmpName += "/force6d:o";
                tmpPort->open(tmpName);
                p_force6d.push_back(tmpPort);
            } else printf("Sensor %d not supported.\n", robotIter);
        }
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

    //-- world rpc server
    worldRpcResponder.setEnvironment(penv);
    worldRpcResponder.setRobot(probot);
    worldRpcServer.open("/ravebot/world");
    worldRpcServer.setReader(worldRpcResponder);

    //-- mobile rpc server
    if(!!pmobile) {
        mobileRpcResponder.setEnvironment(penv);
        mobileRpcResponder.setRobot(probot);
        mobileRpcServer.open("/ravebot/mobile/rpc:i");
        mobileRpcServer.setReader(mobileRpcResponder);
    }

    // Start the RateThread
    this->setRate(jmcMs);
    this->start();
    
    return true;
}

// -----------------------------------------------------------------------------

bool RaveBot::close() {
    printf("RaveBot::close() Closing ports...\n");
    worldRpcServer.interrupt();
    worldRpcServer.close();
    if(!!pmobile) {
        mobileRpcServer.interrupt();
        mobileRpcServer.close();
    }
    penv->StopSimulation();  // NEEDED??
    this->askToStop();
    for (unsigned int i=0;i<p_imagen.size();i++) {
        p_imagen[i]->interrupt();
        p_imagen[i]->close();
        delete p_imagen[i];
    }
    for (unsigned int i=0;i<p_depth.size();i++) {
        p_depth[i]->interrupt();
        p_depth[i]->close();
        delete p_depth[i];
    }
   for (unsigned int i=0;i<p_force6d.size();i++) {
        p_force6d[i]->interrupt();
        p_force6d[i]->close();
        delete p_force6d[i];
    }
    printf("RaveBot::close() Ports closed. Closing environment...\n");
    penv->Destroy(); // destroy
    Time::delay(0.4);
    printf("RaveBot: close() joining...\n");
    orThreads.join_all(); // wait for the viewer thread to exit
    printf("[success] RaveBot::close() Closed.\n");
    return true;
}

// -----------------------------------------------------------------------------

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername)
{
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);

    // attach it to the environment:
    penv->AddViewer(viewer);  // penv->AttachViewer(viewer);

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

// -----------------------------------------------------------------------------

