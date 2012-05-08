
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool CartesianBot::open(Searchable& config) {

    cmc_status = 0;
    startTime = 0;
    realRad.resize(5);
    targetX.resize(3);
    targetO.resize(2);

    A0 = DEFAULT_A0;
    A1 = DEFAULT_A1;
    A2 = DEFAULT_A2;
    A3 = DEFAULT_A3;
    duration = DEFAULT_DURATION;
    maxVel = DEFAULT_MAXVEL;
    maxAcc = DEFAULT_MAXACC;
    cmcMs = DEFAULT_CMC_MS;
    ConstString strRobotDevice = DEFAULT_ROBOTDEVICE;
    ConstString strRobotSubDevice = DEFAULT_ROBOTSUBDEVICE;
    ConstString strRobotName = DEFAULT_ROBOTNAME;

    printf("--------------------------------------------------------------\n");
    if(config.check("help")) {
        printf("CartesianBot options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--A0 [m] (dist from base to motor 2, default: \"%f\")\n",A0);
        printf("\t--A1 [m] (dist from motor 2 to motor 3, default: \"%f\")\n",A1);
        printf("\t--A2 [m] (dist from motor 3 to motor 4, default: \"%f\")\n",A2);
        printf("\t--A3 [m] (dist from motor 4 to end-effector, default: \"%f\")\n",A3);
        printf("\t--duration [s] (duration of movl movements, default: \"%f\")\n",duration);
        printf("\t--maxVel [deg/s] (maximum joint velocity, default: \"%f\")\n",maxVel);
        printf("\t--maxAcc [deg/s^2] (maximum joint acceleration, default: \"%f\")\n",maxAcc);
        printf("\t--cmcMs [ms] (rate of Cartesian Motion Controller thread, default: \"%f\")\n",cmcMs);
        printf("\t--robotDevice (device we create, default: \"%s\")\n",strRobotDevice.c_str());
        printf("\t--robotSubdevice (library we use, default: \"%s\")\n",strRobotSubDevice.c_str());
        printf("\t--robotName (port name of created device, default: \"%s\")\n",strRobotName.c_str());
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (config.check("A0")) A0 = config.find("A0").asDouble();
    if (config.check("A1")) A1 = config.find("A1").asDouble();
    if (config.check("A2")) A2 = config.find("A2").asDouble();
    if (config.check("A3")) A3 = config.find("A3").asDouble();
    printf("CartesianBot using A0: %f, A1: %f, A2: %f, A3: %f.\n",A0,A1,A2,A3);

    if (config.check("duration")) duration = config.find("duration").asDouble();
    if (config.check("maxVel")) maxVel = config.find("maxVel").asDouble();
    if (config.check("maxAcc")) maxAcc = config.find("maxAcc").asDouble();
    if (config.check("cmcMs")) cmcMs = config.find("cmcMs").asDouble();
    printf("CartesianBot using duration: %f, maxVel: %f, maxAcc: %f, cmcMs: %f.\n",duration,maxVel,maxAcc,cmcMs);

    if (config.check("robotDevice")) strRobotDevice = config.find("robotDevice").asString();
    if (config.check("robotSubDevice")) strRobotDevice = config.find("robotSubDevice").asString();
    if (config.check("robotName")) strRobotName = config.find("robotName").asString();
    printf("CartesianBot using robotDevice: %s, robotSubDevice: %s, robotName: %s.\n",strRobotDevice.c_str(),strRobotSubDevice.c_str(),strRobotName.c_str());

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
        printf("[error] CartesianBot problems acquiring robot interfaces\n");
        return false;
    } else printf("[success] CartesianBot acquired robot interfaces\n");

    // Start the RateThread
    this->setRate(cmcMs);
    this->start();
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::close() {
    // printf("Cleaned heap.\n");
    return true;
}

// -----------------------------------------------------------------------------

