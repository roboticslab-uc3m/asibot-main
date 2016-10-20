
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool KdlBot::open(Searchable& config) {

    angleRepr = DEFAULT_ANGLE_REPR; // ConstString
    cmcMs = DEFAULT_CMC_MS;         // double
    duration = DEFAULT_DURATION;    // double
    epsilon = DEFAULT_EPSILON;      // double
    maxVel = DEFAULT_MAXVEL;        // double
    maxAcc = DEFAULT_MAXACC;        // double
    cmcNumMotors = DEFAULT_CMC_NUM_MOTORS; // unsigned int
    ConstString strRobotDevice = DEFAULT_ROBOT_DEVICE;
    ConstString strRobotSubDevice = DEFAULT_ROBOT_SUBDEVICE;
    ConstString strRobotName = DEFAULT_ROBOT_NAME;
    ConstString strRobotLocal = DEFAULT_ROBOT_LOCAL;
    ConstString strRobotRemote = DEFAULT_ROBOT_REMOTE;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("KdlBot options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--angleRepr (axisAngle, eulerYZ, or eulerZYZ (default: \"%s\")\n",angleRepr.c_str());
        printf("\t--cmcMs [ms] (rate of Cartesian Motion Controller thread, default: \"%f\")\n",cmcMs);
        printf("\t--duration [s] (duration of movl movements, default: \"%f\")\n",duration);
        printf("\t--epsilon (epsilon for tolerance, default: \"%f\")\n",epsilon);
        printf("\t--maxAcc [units/s^2] (maximum joint acceleration, default: \"%f\")\n",maxAcc);
        printf("\t--maxVel [units/s] (maximum joint velocity, default: \"%f\")\n",maxVel);
        printf("\t--cmcNumMotors (default: \"%d\")\n",cmcNumMotors);
        printf("\t--robotDevice (device we create, default: \"%s\")\n",strRobotDevice.c_str());
        printf("\t--robotSubdevice (library we use, default: \"%s\")\n",strRobotSubDevice.c_str());
        printf("\t--robotName (port name of created device, default: \"%s\")\n",strRobotName.c_str());
        printf("\t--robotLocal (if accesing remote, local port name, default: \"%s\")\n",strRobotLocal.c_str());
        printf("\t--robotRemote (if accesing remote, remote port name, default: \"%s\")\n",strRobotRemote.c_str());
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (config.check("cmcNumMotors")) cmcNumMotors = config.find("cmcNumMotors").asDouble();
    if (config.check("angleRepr")) angleRepr = config.find("angleRepr").asString();
    printf("KdlBot using cmcNumMotors: %d, angleRepr: %s.\n",cmcNumMotors,angleRepr.c_str());

    if(angleRepr == "axisAngle") {
        targetO.resize(4);
    }else if(angleRepr == "eulerYZ") {
        targetO.resize(2);
    }else if(angleRepr == "eulerZYZ") {
        targetO.resize(3);
    }else printf("[warning] axisAngle not recognized!\n");

    if (config.check("cmcMs")) cmcMs = config.find("cmcMs").asDouble();
    if (config.check("duration")) duration = config.find("duration").asDouble();
    if (config.check("epsilon")) epsilon = config.find("epsilon").asDouble();
    if (config.check("maxAcc")) maxAcc = config.find("maxAcc").asDouble();
    if (config.check("maxVel")) maxVel = config.find("maxVel").asDouble();
    printf("KdlBot using cmcMs: %f, duration: %f, epsilon: %f, maxAcc: %f, maxVel: %f.\n",cmcMs,duration,epsilon,maxAcc,maxVel);

    if (config.check("robotDevice")) strRobotDevice = config.find("robotDevice").asString();
    if (config.check("robotSubDevice")) strRobotSubDevice = config.find("robotSubDevice").asString();
    if (config.check("robotName")) strRobotName = config.find("robotName").asString();
    if (config.check("robotLocal")) strRobotLocal = config.find("robotLocal").asString();
    if (config.check("robotRemote")) strRobotRemote = config.find("robotRemote").asString();
    printf("KdlBot using robotDevice: %s, robotSubDevice: %s, robotName: %s.\n",strRobotDevice.c_str(),strRobotSubDevice.c_str(),strRobotName.c_str());
    printf("KdlBot using robotLocal: %s, robotRemote: %s.\n",strRobotLocal.c_str(),strRobotRemote.c_str());

    yarp::sig::Matrix ymH0(4,4);
    ConstString ycsH0("H0");
    if(!getMatrixFromProperties(config,ycsH0,ymH0)){
        ymH0.eye();
        printf("KdlBot using default H0: H0 = I\n");
    }
    else printf("KdlBot using custom H0:\n%s\n",ymH0.toString().c_str());
    Vector kdlVec0(ymH0(0,3),ymH0(1,3),ymH0(2,3));
    Rotation kdlRot0( ymH0(0,0),ymH0(0,1),ymH0(0,2),ymH0(1,0),ymH0(1,1),ymH0(1,2),ymH0(2,0),ymH0(2,1),ymH0(2,2));
    // H0 = Frame(kdlRot0,kdlVec0);
    theChain.addSegment(Segment(Joint(Joint::None), Frame(kdlRot0,kdlVec0)));

    for(int motor=0;motor<cmcNumMotors;motor++) {
        ConstString link("link_");
        std::ostringstream s;  // link+=ConstString::toString(motor);
        s << motor;
        link += s.str();
        Bottle &bLink=config.findGroup(link);
        if(!bLink.isNull()) printf("KdlBot using %s: ", link.c_str());
        else printf("[warning] No args at: %s\n", link.c_str());
        double linkX = bLink.check("x",Value(0.0)).asDouble();
        double linkY = bLink.check("y",Value(0.0)).asDouble();
        double linkZ = bLink.check("z",Value(0.0)).asDouble();

        ConstString linkType = bLink.check("Type",Value("NULL")).asString();
        if(linkType == "RotX") {
            theChain.addSegment(Segment(Joint(Joint::RotX),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(0);
        } else if(linkType == "RotY") {
            theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(0);
        } else if(linkType == "RotZ") {
            theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(0);
        } else if(linkType == "TransX") {
            theChain.addSegment(Segment(Joint(Joint::TransX),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(1);
        } else if(linkType == "TransY") {
            theChain.addSegment(Segment(Joint(Joint::TransY),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(1);
        } else if(linkType == "TransZ") {
            theChain.addSegment(Segment(Joint(Joint::TransZ),Frame(Vector(linkX,linkY,linkZ))));
            isPrismatic.push_back(1);
        } else printf("\n[warning] link joint type unrecognized!\n");

        printf("%s, x: %f, y: %f, z: %f.\n",linkType.c_str(),linkX,linkY,linkZ);
    }

    yarp::sig::Matrix ymHN(4,4);
    ConstString ycsHN("HN");
    if(!getMatrixFromProperties(config,ycsHN,ymHN)){
        ymHN.eye();
        printf("KdlBot using default HN: HN = I\n");
    }
    else printf("KdlBot using custom HN:\n%s\n",ymHN.toString().c_str());
    Vector kdlVecN(ymHN(0,3),ymHN(1,3),ymHN(2,3));
    Rotation kdlRotN( ymHN(0,0),ymHN(0,1),ymHN(0,2),ymHN(1,0),ymHN(1,1),ymHN(1,2),ymHN(2,0),ymHN(2,1),ymHN(2,2));
    // HN = Frame(kdlRotN,kdlVecN);
    theChain.addSegment(Segment(Joint(Joint::None), Frame(kdlRotN,kdlVecN)));
    
    printf("KdlBot chain number of segments including none-joint (H0 and HN): %d\n",theChain.getNrOfSegments());

    if( (strRobotDevice=="remote_controlboard") && (config.check("help")) ) {
        printf("--------------------------------------------------------------\n");
        ::exit(1);
    }

    cmc_status = 0;
    startTime = 0;

    Property options;
    options.fromString(config.toString());
    options.put("device",strRobotDevice);
    options.put("subdevice",strRobotSubDevice);
    options.put("name",strRobotName);
    options.put("local",strRobotLocal);
    options.put("remote",strRobotRemote);

    robotDevice.open(options);
    if (!robotDevice.isValid()) {
        // printf("Robot device not available. Here are the known devices:\n");
        // printf("%s", Drivers::factory().toString().c_str());
        fprintf(stderr,"Robot device not available.\n");
        return 0;
    }

    bool ok;
    ok = robotDevice.view(pos);
    ok = ok && robotDevice.view(vel);
    ok = ok && robotDevice.view(enc);
    ok = ok && robotDevice.view(lim);
    if (!ok) {
        printf("[error] KdlBot problems acquiring robot interfaces\n");
        return false;
    } else printf("[success] KdlBot acquired robot interfaces\n");

    int ax; 
    pos->getAxes(&ax);
    printf("[success] KdlBot pull joint limits:\n");
    double min,max;
    for (int i=0;i<ax;i++){
        lim->getLimits(i,&min,&max);
        printf("[KdlBot] q%d: %f to %f\n",i+1,min,max);
    }

    cmc_status = 0;
    _orient = new RotationalInterpolation_SingleAxis();
    _eqradius = 1; //0.000001;
    _aggregate = false;

    // Start the RateThread
    this->setRate(cmcMs);
    this->start();
    return true;
}

// -----------------------------------------------------------------------------

bool KdlBot::close() {
    delete _orient;
    printf("[KdlBot] Cleaned heap.\n");
    return true;
}

// -----------------------------------------------------------------------------

