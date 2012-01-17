
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool CartesianBot::open(Searchable& config) {

    if(config.check("help")) {
       printf("\n");
       printf("Usage: cartesianbot --help  -------> This help\n");
       exit(1);
    }

    // 5 DOF chain for FK solver
    // Note that does not include length of first link.
    theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.12))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.4))));
    theChain.addSegment(Segment(Joint(Joint::RotY),Frame(Vector(0.0,0.0,0.12))));
    theChain.addSegment(Segment(Joint(Joint::RotZ),Frame(Vector(0.0,0.0,0.16))));
    ChainFkSolverPos_recursive fksolver = ChainFkSolverPos_recursive(theChain);
    ChainFkSolverPos_recursive fksolver1(theChain);  // Forward position solver.
    real_rad = JntArray(5);
    fksolver.JntToCart(real_rad,real_cartpos);
    fksolver.JntToCart(real_rad,target_cartpos);

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
        printf("Problems acquiring interfaces\n");
        return false;
    }
    printf("OK acquiring interfaces\n");

    // Start the RateThread
    this->start();
   
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::close() {
    return true;
}

// -----------------------------------------------------------------------------

