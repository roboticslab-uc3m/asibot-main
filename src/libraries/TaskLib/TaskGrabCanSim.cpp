// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskGrabCanSim.hpp"

/************************************************************************/
bool TaskGrabCanSim::configure(yarp::os::Searchable& config) {
    if (!_quiet) printf("[TaskGrabCanSim] success: begin{configure()}\n");

    _quiet = false;
    if (config.check("quiet")) _quiet = true;
    if (config.check("robotAngle")) _robotAngle = config.find("robotAngle").asDouble();

    if (!_quiet) printf("[TaskGrabCanSim] Using robotAngle: %f.\n", _robotAngle);
    if (!_quiet) printf("[TaskGrabCanSim] success: end{configure()}\n");    
    return true;
}

/************************************************************************/
bool TaskGrabCanSim::run() {
    if (!_quiet) printf("[TaskGrabCanSim] success: begin{run()}\n");
    if (!_cartesianClient.open("/ravebot")) return false;
    if (!_worldRpcClient.open("/taskGrabCanSim/world:o")) return false;
    if (!_worldRpcClient.addOutput("/ravebot/world")) return false;
    //
    Bottle bOut, bIn;
    bOut.addString("world");
    bOut.addString("whereis");
    bOut.addString("obj");
    bOut.addString("redCan");
    _worldRpcClient.write(bOut, bIn);
    Bottle* data = bIn.get(0).asList();
    double redCan[3] = {data->get(0).asDouble(), data->get(1).asDouble(), data->get(2).asDouble()};
    //if (!_quiet) printf("[TaskGrabCanSim] Got info on redCan: %s\n",bIn.toString().c_str());
    if (!_quiet) printf("[TaskGrabCanSim] world says redCan at: %f %f %f.\n",redCan[0],redCan[1],redCan[2]);
    //
    bOut.clear(); bIn.clear();
    bOut.addString("world");
    bOut.addString("whereis");
    bOut.addString("tcp");
    _worldRpcClient.write(bOut, bIn);
    data = bIn.get(0).asList();
    double tcp[3] = {data->get(0).asDouble(), data->get(1).asDouble(), data->get(2).asDouble()};
    if (!_quiet) printf("[TaskGrabCanSim] world says tcp at: %f %f %f.\n",tcp[0],tcp[1],tcp[2]);
    //
    double solverTcp[5];
    _cartesianClient.stat(solverTcp);
    if (!_quiet) printf("[TaskGrabCanSim] solver says tcp at: %f %f %f.\n",solverTcp[0],solverTcp[1],solverTcp[2]);
    //
    double solverRedCan[3] = {-9999,-9999,-9999};
    //_cartesianClient.stat(solverTcp);
    if (!_quiet) printf("[TaskGrabCanSim] redCan in solver coordinates: %f %f %f.\n",solverRedCan[0],solverRedCan[1],solverRedCan[2]);

    /*{
        double targets[5] = {0,-0.3,0.9,90,0};
        printf("[TaskGrabCanSim] Commanding Movj to targets: {0,-0.3, 0.9, 90.0, 0.0}...\n");
        _cartesianClient.movj(targets);
    }*/
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

