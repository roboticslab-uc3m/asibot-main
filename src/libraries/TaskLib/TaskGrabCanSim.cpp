// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskGrabCanSim.hpp"

/************************************************************************/
bool TaskGrabCanSim::configure(yarp::os::Searchable& config) {
    if (!_quiet) printf("[TaskGrabCanSim] success: begin{configure()}\n");

    _quiet = false;
    if (config.check("quiet")) _quiet = true;
    if (config.check("robotAngle")) _robotAngle = config.find("robotAngle").asDouble();

    if (!_quiet) 
    if (!_quiet) printf("[TaskGrabCanSim] success: end{configure()}\n");    
    return true;
}

/************************************************************************/
bool TaskGrabCanSim::run() {
    if (!_quiet) printf("[TaskGrabCanSim] success: begin{run()}\n");
    if (!_cartesianClient.open("/ravebot")) return false;
    if (!_worldRpcClient.open("/taskGrabCanSim/world:o")) return false;
    if (!_worldRpcClient.addOutput("/ravebot/world")) return false;
    Bottle bOut, bIn;
    bOut.addString("world");
    bOut.addString("whereis");
    bOut.addString("obj");
    bOut.addString("redCan");
    _worldRpcClient.write(bOut, bIn);
    //printf("[TaskGrabCanSim] Got info on redCan: %s\n",bIn.toString().c_str());
    Bottle* data = bIn.get(0).asList();
    double x = data->get(0).asDouble();
    double y = data->get(1).asDouble();
    double z = data->get(2).asDouble();
    printf("[TaskGrabCanSim] world says redCan at %f %f %f.\n",x,y,z);


    {
        double targets[5] = {0,-0.3,0.9,90,0};
        printf("[TaskGrabCanSim] Commanding Movj to targets: {0,-0.3, 0.9, 90.0, 0.0}...\n");
        _cartesianClient.movj(targets);
    }
    if (!_quiet) printf("[TaskGrabCanSim] success: end{run()}\n");
    return true;
}

/************************************************************************/
bool TaskGrabCanSim::close() {
    if (!_quiet) printf("[TaskGrabCanSim] success: begin{close()}\n");
    _worldRpcClient.interrupt();
    _cartesianClient.close();
    if (!_quiet) printf("[TaskGrabCanSim] success: end{close()}\n");
    return true;
}

/************************************************************************/

