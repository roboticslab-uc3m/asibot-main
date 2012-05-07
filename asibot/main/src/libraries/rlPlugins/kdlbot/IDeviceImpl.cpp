
// -----------------------------------------------------------------------------

#include "KdlBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool KdlBot::open(Searchable& config) {

    numMotors = DEFAULT_NUM_MOTORS;     // unsigned int
    angleRepr = DEFAULT_ANGLE_REPR;     // ConstString
    duration = DEFAULT_DURATION;        // double
    maxVel = DEFAULT_MAXVEL;            // double
    maxAcc = DEFAULT_MAXACC;            // double
    msCartesian = DEFAULT_MSCARTESIAN;  // double
    ConstString strRobotDevice = DEFAULT_ROBOTDEVICE;
    ConstString strRobotSubDevice = DEFAULT_ROBOTSUBDEVICE;
    ConstString strRobotName = DEFAULT_ROBOTNAME;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("KdlBot options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--angleRepr (axisAngle or eYZ. default: \"%s\")\n",angleRepr.c_str());
        printf("\t--numMotors (default: \"%d\")\n",numMotors);
        printf("\t--duration [s] (duration of movl movements, default: \"%f\")\n",duration);
        printf("\t--maxVel [deg/s] (maximum joint velocity, default: \"%f\")\n",maxVel);
        printf("\t--maxAcc [deg/s^2] (maximum joint acceleration, default: \"%f\")\n",maxAcc);
        printf("\t--msCartesian [ms] (rate of cartesian control thread, default: \"%f\")\n",msCartesian);
        printf("\t--robotDevice (device we create, default: \"%s\")\n",strRobotDevice.c_str());
        printf("\t--robotSubdevice (library we use, default: \"%s\")\n",strRobotSubDevice.c_str());
        printf("\t--robotName (port name of created device, default: \"%s\")\n",strRobotName.c_str());
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (config.check("numMotors")) numMotors = config.find("numMotors").asDouble();
    if (config.check("angleRepr")) angleRepr = config.find("angleRepr").asString();
    printf("KdlBot using numMotors: %d, angleRepr: %s.\n",numMotors,angleRepr.c_str());

    targetX.resize(3);
    if(angleRepr == "eYZ") {
        targetO.resize(2);
    }else if(angleRepr == "axisAngle") {
        targetO.resize(4);
    }else printf("[warning] axisAngle not recognized!\n");

    if (config.check("duration")) duration = config.find("duration").asDouble();
    if (config.check("maxVel")) maxVel = config.find("maxVel").asDouble();
    if (config.check("maxAcc")) maxAcc = config.find("maxAcc").asDouble();
    if (config.check("msCartesian")) msCartesian = config.find("msCartesian").asDouble();
    printf("KdlBot using duration: %f, maxVel: %f, maxAcc: %f, msCartesian: %f.\n",duration,maxVel,maxAcc,msCartesian);

    if (config.check("robotDevice")) strRobotDevice = config.find("robotDevice").asString();
    if (config.check("robotSubDevice")) strRobotDevice = config.find("robotSubDevice").asString();
    if (config.check("robotName")) strRobotName = config.find("robotName").asString();
    printf("KdlBot using robotDevice: %s, robotSubDevice: %s, robotName: %s.\n",strRobotDevice.c_str(),strRobotSubDevice.c_str(),strRobotName.c_str());

    for(int motor=0;motor<numMotors;motor++) {
        ConstString link("link_");
        link+=ConstString::toString(motor);
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
    yarp::sig::Matrix yM(4,4);
    ConstString yCS("HN");
    if(!getMatrixFromProperties(config,yCS,yM)){
        yM.eye();
        printf("KdlBot using default HN:\n%s\n",yM.toString().c_str());
    }
    else printf("KdlBot using custom HN:\n%s\n",yM.toString().c_str());
    Vector kdlVec(yM(0,3),yM(1,3),yM(2,3));
    Rotation kdlRot( yM(0,0),yM(0,1),yM(0,2),yM(1,0),yM(1,1),yM(1,2),yM(2,0),yM(2,1),yM(2,2));
    HN = Frame(kdlRot,kdlVec);

    cmc_status = 0;
    startTime = 0;

    Property options(config.toString());
    options.put("device",strRobotDevice);
    options.put("subdevice",strRobotSubDevice);
    options.put("name",strRobotName);

    robotDevice.open(options);
    if (!robotDevice.isValid()) {
        printf("Robot device not available. Here are the known devices:\n");
        printf("%s", Drivers::factory().toString().c_str());
        return 0;
    }

    bool ok;
    ok = robotDevice.view(pos);
    ok = ok && robotDevice.view(vel);
    ok = ok && robotDevice.view(enc);
    if (!ok) {
        printf("[error] KdlBot problems acquiring robot interfaces\n");
        return false;
    } else printf("[success] KdlBot acquired robot interfaces\n");

    cmc_status = 0;
    _orient = new RotationalInterpolation_SingleAxis();
    _eqradius = 1; //0.000001;
    _aggregate = false;

    // Start the RateThread
    this->setRate(msCartesian);
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

