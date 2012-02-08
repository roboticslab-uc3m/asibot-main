
// -----------------------------------------------------------------------------

#include "RaveBot.h"

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername);

// ------------------- DeviceDriver Related ------------------------------------

bool RaveBot::open(Searchable& config) {

    if(config.check("help")) {
       printf("\n");
       printf("Option: --help  -------> This help\n");
       printf("Option: --env [env]  --> [env] env name (abs, or rel to ASIBOT_ROOT, defaults to %s)\n",DEFAULT_ENV_NAME);
       printf("\n");
       exit(1);
    }

    // Create the name of the scene to load, three options:
    //   1) User gives us nothing -> we use $ASIBOT_ROOT/default
    //   2) User gives us abs -> we use "env"
    //   2) User gives us rel -> we use $ASIBOT_ROOT/"env"
    char *c_yarpmodsdir;
    c_yarpmodsdir = getenv("ASIBOT_ROOT");
    if(c_yarpmodsdir) {
      printf("$ASIBOT_DIR is set to: %s\n",c_yarpmodsdir);
    } else {
      printf("[warning] $ASIBOT_ROOT is not set, should look like ~/asibot ...\n\n");
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
      refSpeed[i]=DEFAULT_REFSPEED;
      refAcc[i]=1.0;
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

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true;
    viewer->main(showgui);
}

