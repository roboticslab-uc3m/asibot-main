// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RaveBot.h"

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername, int _viewer);

// ------------------- DeviceDriver Related ------------------------------------

bool RaveBot::open(Searchable& config) {

    numMotors = DEFAULT_NUM_MOTORS;
    jmcMs = DEFAULT_JMC_MS;
    jmcMsAcc = DEFAULT_JMC_MS_ACC;

    ConstString env = DEFAULT_ENV;
    ConstString externObj = DEFAULT_EXTERN_OBJ;
    ConstString extraRobot = DEFAULT_EXTRA_ROBOT;
    double genInitPos = DEFAULT_GEN_INIT_POS;
    double genJointTol = DEFAULT_GEN_JOINT_TOL;
    double genMaxLimit = DEFAULT_GEN_MAX_LIMIT;
    double genMinLimit = DEFAULT_GEN_MIN_LIMIT;
    double genRefSpeed = DEFAULT_GEN_REF_SPEED;
    double genEncRawExposed = DEFAULT_GEN_ENC_RAW_EXPOSED;
    double genVelRawExposed = DEFAULT_GEN_VEL_RAW_EXPOSED;
    modePosVel = DEFAULT_MODE_POS_VEL;
    ConstString physics = DEFAULT_PHYSICS;
    viewer = DEFAULT_VIEWER;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("RaveBot options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--numMotors [int] (number of motors to control, default: \"%d\")\n",numMotors);

        printf("\t--env [xml] (env rel to \"share/ravebot\", default: \"%s\")\n",env.c_str());
        printf("\t--externObj (default: \"%s\")\n",externObj.c_str());
        printf("\t--extraRobot (type of extra robot, default: \"%s\")\n",extraRobot.c_str());
        printf("\t--genInitPos [units] (default: \"%f\")\n",genInitPos);
        printf("\t--genJointTol [units] (default: \"%f\")\n",genJointTol);
        printf("\t--genMaxLimit [units] (default: \"%f\")\n",genMaxLimit);
        printf("\t--genMinLimit [units] (default: \"%f\")\n",genMinLimit);
        printf("\t--genRefSpeed [units/s] (default: \"%f\")\n",genRefSpeed);
        printf("\t--genEncRawExposed (encoder [raw / exposed] ratio, default: \"%f\")\n",genEncRawExposed);
        printf("\t--genVelRawExposed (velocity [raw / exposed] ratio, default: \"%f\")\n",genVelRawExposed);
        printf("\t--jmcMs [ms] (rate of Joint Motion Controller thread, default: \"%f\")\n",jmcMs);
        printf("\t--modePosVel (default: \"%d\")\n",modePosVel);
        printf("\t--physics [type] (type of physics, default: \"%s\")\n",physics.c_str());
        printf("\t--viewer [type] (set to 0 for none, default: \"%d\")\n",viewer);
    }

    if (config.check("numMotors")) numMotors = config.find("numMotors").asDouble();
    if (config.check("env")) env = config.find("env").asString();
    if (config.check("externObj")) externObj = config.find("externObj").asString();
    if (config.check("extraRobot")) extraRobot = config.find("extraRobot").asString();
    printf("RaveBot using numMotors: %d, env: %s, externObj: %s, extraRobot: %s.\n",
        numMotors, env.c_str(), externObj.c_str(), extraRobot.c_str());

    if (config.check("genInitPos")) genInitPos = config.find("genInitPos").asDouble();
    if (config.check("genJointTol")) genJointTol = config.find("genJointTol").asDouble();
    if (config.check("genMaxLimit")) genMaxLimit = config.find("genMaxLimit").asDouble();
    if (config.check("genMinLimit")) genMinLimit = config.find("genMinLimit").asDouble();
    if (config.check("genRefSpeed")) genRefSpeed = config.find("genRefSpeed").asDouble();
    if (config.check("genEncRawExposed")) genEncRawExposed = config.find("genEncRawExposed").asDouble();
    if (config.check("genVelRawExposed")) genVelRawExposed = config.find("genVelRawExposed").asDouble();    
    if (config.check("jmcMs")) jmcMs = config.find("jmcMs").asDouble();
    if (config.check("jmcMsAcc")) jmcMsAcc = config.find("jmcMsAcc").asDouble();
    if (config.check("modePosVel")) modePosVel = config.find("modePosVel").asInt();
    if (config.check("physics")) physics = config.find("physics").asString();
    if (config.check("viewer")) viewer = config.find("viewer").asInt();
    printf("RaveBot using genInitPos: %f, genJointTol: %f, genMaxLimit: %f, genMinLimit: %f.\n",
        genInitPos, genJointTol,genMaxLimit,genMinLimit);
    printf("RaveBot using genRefSpeed: %f, genEncRawExposed: %f, genVelRawExposed: %f.\n",
        genRefSpeed,genEncRawExposed, genVelRawExposed);
    printf("RaveBot using jmcMs: %f, jmcMsAcc: %f, modePosVel: %d, physics: %s, viewer: %d.\n",
        jmcMs,jmcMsAcc,modePosVel,physics.c_str(),viewer);

    Bottle* initPoss;
    if (config.check("initPoss")) {
        initPoss = config.find("initPoss").asList();
        printf("RaveBot using individual initPoss: %s\n",initPoss->toString().c_str());
        if(initPoss->size() != int(numMotors)) printf("[warning] initPoss->size() != numMotors\n");
    } else {
        initPoss = 0;
        printf("RaveBot not using individual initPoss, defaulting to genInitPos.\n");
    }
    Bottle* jointTols;
    if (config.check("jointTols")) {
        jointTols = config.find("jointTols").asList();
        printf("RaveBot using individual jointTols: %s\n",jointTols->toString().c_str());
        if(jointTols->size() != int(numMotors)) printf("[warning] jointTols->size() != numMotors\n");
    } else {
        jointTols = 0;
        printf("RaveBot not using individual jointTols, defaulting to genJointTol.\n");
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
        ::exit(1);
    }

    encRawExposed.resize(numMotors);
    jointStatus.resize(numMotors);
    initPos.resize(numMotors);
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
        if(!initPoss) initPos[i]=genInitPos;
        else initPos[i]=initPoss->get(i).asDouble(); 
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

    for (unsigned int i=0; i<numMotors; i++) {
        setEncoder(i,initPos[i]);
    }

    // Initialize OpenRAVE-core
    RaveInitialize(true);  // Start openrave core
    penv = RaveCreateEnvironment();  // Create the main OpenRAVE environment, set the EnvironmentBasePtr
    penv->SetDebugLevel(Level_Debug);  // Relatively new function
    penv->StopSimulation();  // NEEDED??
    boost::thread thviewer(boost::bind(SetViewer,penv,"qtcoin",viewer));
    orThreads.add_thread(&thviewer);
    Time::delay(0.4); // wait for the viewer to init, in [s]


    // Actually load the scene
    yarp::os::ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("ravebot");
    std::string envFull( rf.findFileByName(env) );

    if (! penv->Load(envFull.c_str()) ) {
        printf("[error] RaveBot could not load %s environment.\n",envFull.c_str());
        return false;
    }
    printf("[success] RaveBot loaded environment.\n");

    // Attach a physics engine
    if(physics=="ode"){
        penv->SetPhysicsEngine(RaveCreatePhysicsEngine(penv,"ode"));
        penv->GetPhysicsEngine()->SetGravity(OpenRAVE::Vector(0,0,-9.8));
    }

    //-- Get the robot
    std::vector<RobotBasePtr> robots;
    penv->GetRobots(robots);
    //-- Robot 0
    probot = robots.at(0);  // which is a RobotBasePtr
    printf("RaveBot using robot 0 (%s) as main robot.\n", probot->GetName().c_str());
    //std::vector<RobotBase::ManipulatorPtr>::const_iterator myIt = probot->GetManipulators().begin();
    probotManip = probot->GetManipulators()[0];
    worldRpcResponder.robotDraw = 0;
    worldRpcResponder.drawRadius = 0.005;
    worldRpcResponder.drawR = 1;
    worldRpcResponder.drawG = 1;
    worldRpcResponder.drawB = 1;
    drawCounter = 0;
    drawnElems = 0;
//	    myIt != pRobot->GetManipulators().end(); myIt++) {
//      printf("Manip: %s\n",(*myIt)->GetName().c_str());  // get "asibotManipulator"

    //-- Robot 1
    if(extraRobot=="mobile") {
        pmobile = robots.at(1);  // which is a RobotBasePtr
        printf("RaveBot using robot 1 (%s) as mobile robot.\n", pmobile->GetName().c_str());
        pndof = RobotBasePtr();  // null boost pointer
    } else if ((extraRobot=="1dof")||(extraRobot=="2dof")||(extraRobot=="3dof")) {
        pndof = robots.at(1);  // which is a RobotBasePtr
        printf("RaveBot using robot 1 (%s) as ndof robot.\n", pndof->GetName().c_str());
        pmobile = RobotBasePtr();  // null boost pointer
        if(extraRobot=="1dof") extraCallbackPort.dof = 1;
        else if(extraRobot=="2dof") extraCallbackPort.dof = 2;
        else if(extraRobot=="3dof") extraCallbackPort.dof = 3;
    } else {
        pmobile = RobotBasePtr();  // null boost pointer
        pndof = RobotBasePtr();  // null boost pointer
    }

    for ( unsigned int robotIter = 0; robotIter<robots.size(); robotIter++ ) {
        std::vector<RobotBase::AttachedSensorPtr> sensors = robots.at(robotIter)->GetAttachedSensors();
        printf("Sensors found on robot %d (%s): %d.\n",robotIter,robots.at(robotIter)->GetName().c_str(),(int)sensors.size());
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
                boost::shared_ptr<SensorBase::CameraGeomData const> pcamerageomdata = boost::dynamic_pointer_cast<SensorBase::CameraGeomData const>(psensorbase->GetSensorGeometry(SensorBase::ST_Camera));
                // printf("Camera width: %d, height: %d.\n",pcamerageomdata->width,pcamerageomdata->height);
                cameraWidth.push_back(pcamerageomdata->width);
                cameraHeight.push_back(pcamerageomdata->height);
                // Get a pointer to access the camera data stream
                pcamerasensordata.push_back(boost::dynamic_pointer_cast<SensorBase::CameraSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Camera)));
                pcamerasensorbase.push_back(psensorbase);  // "save"
                BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* tmpPort = new BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >;
                ConstString tmpName("/ravebot/");
                ConstString cameraSensorString(psensorbase->GetName());
                size_t pos = cameraSensorString.find("imageMap");
                if ( pos != std::string::npos) {
                    tmpName += cameraSensorString.substr (0,pos-1);
                    tmpName += "/imageMap:o";
                } else {
                    tmpName += cameraSensorString.c_str();
                    tmpName += "/img:o";
                }
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
                BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* tmpPort = new BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >;
                ConstString tmpName("/ravebot/");
                ConstString depthSensorString(psensorbase->GetName());
                size_t pos = depthSensorString.find("depthMap");
                if ( pos != std::string::npos) {
                    tmpName += depthSensorString.substr (0,pos-1);
                    tmpName += "/depthMap:o";
                } else {
                    tmpName += depthSensorString.c_str();
                    tmpName += "/depth:o";
                }
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
    worldRpcResponder.setRobotManip(probotManip);
    worldRpcServer.open("/ravebot/world");
    worldRpcServer.setReader(worldRpcResponder);

    //-- mobile rpc server
    if(extraRobot=="mobile") {
        KinBodyPtr objPtr = penv->GetKinBody("asibot");
        if(objPtr) {
            if(pmobile->Grab(objPtr)) printf("[success] object asibot exists and grabbed.\n");
            else printf("[warning] object asibot exists but not grabbed.\n");
        } else printf("[fail] object asibot does not exist.\n");
        //
        vector<int> vindices;  // send empty vector instead of joints
        //pmobile->SetActiveDOFs(vindices,DOF_X|DOF_Y,Vector(0,0,1));  // and grab world pos
        pmobile->SetActiveDOFs(vindices,DOF_X|DOF_Y|DOF_RotationAxis,Vector(0,0,1));  // and grab world pos
        pbasemanip = RaveCreateModule(penv,"basemanipulation"); // create the module
        penv->Add(pbasemanip,true,pmobile->GetName()); // load the module
        mobileRpcResponder.setEnvironment(penv);
        mobileRpcResponder.setMobile(pmobile);
        mobileRpcResponder.setModule(pbasemanip);
        mobileRpcServer.open("/ravebot/mobile/rpc:i");
        mobileRpcServer.setReader(mobileRpcResponder);
    } else if((extraRobot=="1dof")||(extraRobot=="2dof")||(extraRobot=="3dof")) {
        ConstString ndofPortStr("/ravebot/");
        ndofPortStr += extraRobot;
        ndofPortStr += "/command:i";
        extraCallbackPort.open(ndofPortStr);
        extraCallbackPort.useCallback();
    }

    if(externObj=="redCan") {
        RaveLoadPlugin("ExternObj");
        ModuleBasePtr pExternObj = RaveCreateModule(penv,"ExternObj"); // create the module
        penv->Add(pExternObj,true); // load the module, calls main and also enables good destroy.
        std::stringstream cmdin,cmdout;
        cmdin << "Open";  // default maxiter:4000
        RAVELOG_INFO("%s\n",cmdin.str().c_str());
        if( !pExternObj->SendCommand(cmdout,cmdin) ) {
            fprintf(stderr,"Bad send Open command.\n");
        }
        printf("Sent Open command.\n");
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
    if(!!pndof) {
        extraCallbackPort.disableCallback();
        extraCallbackPort.interrupt();
        extraCallbackPort.close();
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

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername, int _viewer)
{
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);

    // attach it to the environment:
    penv->AddViewer(viewer);  // penv->AttachViewer(viewer);

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true; // change to false to disable scene view
    if(!_viewer) showgui = false;  // if viewer arg = 0
    viewer->main(showgui);
}

// -----------------------------------------------------------------------------

