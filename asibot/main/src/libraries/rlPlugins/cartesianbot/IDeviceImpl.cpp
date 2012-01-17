
// -----------------------------------------------------------------------------

#include "CartesianBot.h"

// ------------------- DeviceDriver Related ------------------------------------

bool CartesianBot::open(Searchable& config) {

    printf("HI I WORK!!!!!\n");

    if(config.check("help")) {
       printf("\n");
       printf("Usage: cartesianbot --help  -------> This help\n");
       exit(1);
    }

    // Start the RateThread
    //this->start();
    
    return true;
}

// -----------------------------------------------------------------------------

bool CartesianBot::close() {
    return true;
}

// -----------------------------------------------------------------------------

