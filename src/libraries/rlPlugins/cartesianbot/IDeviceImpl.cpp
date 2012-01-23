
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool CartesianBot::open(Searchable& config) {

    if(config.check("help")) {
        printf("\n");
        printf("Usage: cartesianbot --help  -------> This help\n");
        exit(1);
    }

    if((!config.check("A1"))||(!config.check("A2"))||(!config.check("A3"))){
        printf("[error] A1,A2,A3 must be defined in [link_lenghts] from .ini\n");
        return false;
    }
    A1 = config.find("A1").asInt();
    A2 = config.find("A2").asInt();
    A3 = config.find("A3").asInt();

    realRad = JntArray(5);

    vgeneral = 100;
    cmc_status = 0;
    _orient = new RotationalInterpolation_SingleAxis();
    _eqradius = 1; //0.000001;
    _aggregate = false;
    duration = DEFAULT_DURATION;
    currentTime = 0;

    Property options;
/////// UNCOMMENT NEXT 3 LINES FOR USING RAVEBOT AS A LIBRARY AND OPENING FOR REMOTE ////////
    options.put("device","controlboard");
    options.put("subdevice","ravebot");
    options.put("name","/ravebot");
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
    }
    printf("[success] OK acquiring interfaces\n");

    // Start the RateThread
    int period = config.check("rate",30,"ms ratethread").asInt();
    this->setRate(period);
    this->start();
    printf("[success] Started %d ms ratethread\n",period);
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::close() {
    delete _orient;
    _orient = 0;
//    delete pFksolver;
//    pFksolver = 0;
    printf("Cleaned heap.\n");
    return true;
}

// -----------------------------------------------------------------------------

