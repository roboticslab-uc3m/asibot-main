// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskDrinkSim.hpp"

/************************************************************************/
bool TaskDrinkSim::configure(yarp::os::Searchable& config) {
    if (!_quiet) printf("[TaskDrinkSim] success: begin{configure()}\n");

    _quiet = false;
    if (config.check("quiet")) _quiet = true;
    if (config.check("robotAngle")) _robotAngle = config.find("robotAngle").asDouble();

    if (!_quiet) printf("[TaskDrinkSim] Using robotAngle: %f.\n", _robotAngle);
    if (!_quiet) printf("[TaskDrinkSim] success: end{configure()}\n");    
    return true;
}

/************************************************************************/
bool TaskDrinkSim::run() {
    if (!_quiet) printf("[TaskDrinkSim] success: begin{run()}\n");
    if (!_cartesianClient.open("/ravebot")) return false;
    if (!_worldRpcClient.open("/taskDrinkSim/world:o")) return false;
    if (!_worldRpcClient.addOutput("/ravebot/world")) return false;
    //
    {
        double targets[5] = {-.4, -.4, .6, 90, 0};
        printf("[TaskDrinkSim] Commanding Movj to targets: {-.4, -.4, .6, 90, 0}... [wait]\n");
        _cartesianClient.movj(targets);
        _cartesianClient.wait();
    }
    //
    {
        double targets[5] = {.26,-.14,.58,110,0};
        printf("[TaskDrinkSim] Commanding Movj to targets: {.26,-.14,.58,100,0}... [wait]\n");
        _cartesianClient.movj(targets);
        _cartesianClient.wait();
    }
    {
        double targets[5] = {.26,-.14,.58,110,-60};
        printf("[TaskDrinkSim] Commanding Movj to targets: {.26,-.14,.58,100,-60}... [wait]\n");
        _cartesianClient.movj(targets);
        _cartesianClient.wait();
    }
    {
        double targets[5] = {.26,-.14,.58,110,0};
        printf("[TaskDrinkSim] Commanding Movj to targets: {.26,-.14,.58,100,-60}... [wait]\n");
        _cartesianClient.movj(targets);
        _cartesianClient.wait();
    }
    {
        double targets[5] = {-.4, -.4, .6, 90, 0};
        printf("[TaskDrinkSim] Commanding Movj to targets: {-.4, -.4, .6, 90, 0}... [wait]\n");
        _cartesianClient.movj(targets);
        _cartesianClient.wait();
    }
    //
    if (!_quiet) printf("[TaskDrinkSim] success: end{run()}\n");
    return true;
}

/************************************************************************/
bool TaskDrinkSim::close() {
    if (!_quiet) printf("[TaskDrinkSim] success: begin{close()}\n");
    _worldRpcClient.interrupt();
    _cartesianClient.close();
    _worldRpcClient.close();
    if (!_quiet) printf("[TaskDrinkSim] success: end{close()}\n");
    return true;
}

/************************************************************************/

