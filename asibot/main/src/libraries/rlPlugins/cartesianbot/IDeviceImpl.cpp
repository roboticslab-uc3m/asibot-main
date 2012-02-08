
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool CartesianBot::open(Searchable& config) {

    if(config.check("help")) {
        printf("\n");
        printf("Usage: cartesianbot --help  -------> This help\n");
        exit(1);
    }

    if((!config.check("A0"))||(!config.check("A1"))||(!config.check("A2"))||(!config.check("A3"))){
        printf("[error] A0,A1,A2,A3 must be defined to open() CartesianBot\n");
        return false;
    }
    A0 = config.find("A0").asDouble();
    A1 = config.find("A1").asDouble();
    A2 = config.find("A2").asDouble();
    A3 = config.find("A3").asDouble();
    printf("Using A0:%f, A1:%f, A2:%f, A3:%f.\n",A0,A1,A2,A3);

    realRad.resize(5);
    targetX.resize(3);
    targetO.resize(2);

    cmc_status = 0;
    startTime = 0;
    duration = DEFAULT_DURATION;
    maxVel = DEFAULT_MAXVEL;
    maxAcc = DEFAULT_MAXACC;

    if(!config.check("robotDevice")){
        printf("[error] robotDevice must be defined to open() CartesianBot\n");
        printf("[error] you may also define subdevice,name,local,remote to open() CartesianBot\n");
        return false;
    }
    Property options;
    options.put("device",config.find("robotDevice").asString());
    options.put("subdevice",config.find("robotSubdevice").asString());
    options.put("name",config.find("robotName").asString());
    options.put("local",config.find("robotLocal").asString());
    options.put("remote",config.find("robotRemote").asString());

/////// UNCOMMENT NEXT 3 LINES FOR USING RAVEBOT AS A LIBRARY AND OPENING FOR REMOTE ////////
//    options.put("device","controlboard");
//    options.put("subdevice","ravebot");
//    options.put("name","/ravebot");
//////// UNCOMMENT NEXT 1 LINE FOR USING RAVEBOT AS A LIBRARY ///////////////////////
//        options.put("device","ravebot");
//////// UNCOMMENT NEXT 3 LINES FOR CONNECTING TO REMOTE RAVEBOT /////////////
//        options.put("device","remote_controlboard");
//        options.put("local","/local");
//        options.put("remote","/ravebot");

    robotDevice.open(options);
    if (!robotDevice.isValid()) {
        printf("Device not available.  Here are the known devices:\n");
        printf("%s", Drivers::factory().toString().c_str());
        return 0;
    }
    bool ok;
    ok = robotDevice.view(pos);
    ok = ok && robotDevice.view(vel);
    ok = ok && robotDevice.view(enc);
    if (!ok) {
        printf("[error] Problems acquiring interfaces\n");
        return false;
    } else printf("[success] cartesianbot acquired robot interfaces\n");

    // Start the RateThread
    msPeriod = config.check("rate",30,"ms ratethread").asDouble();
    this->setRate(msPeriod);
    this->start();
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::close() {
    // printf("Cleaned heap.\n");
    return true;
}

// -----------------------------------------------------------------------------

