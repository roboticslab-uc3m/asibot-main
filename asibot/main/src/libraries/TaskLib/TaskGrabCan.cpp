// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskGrabCan.hpp"

/************************************************************************/
bool TaskGrabCan::configure(yarp::os::Searchable& config) {
    if (!_quiet) printf("[TaskGrabCan] success: begin{configure()}\n");

    _quiet = false;
    if (config.check("quiet")) _quiet = true;

    if (!_quiet) printf("[TaskGrabCan] success: end{configure()}\n");    
    return true;
}

/************************************************************************/
bool TaskGrabCan::run() {
    if (!_quiet) printf("[TaskGrabCan] success: begin{run()}\n");
    {
        double targets[5] = {0,-0.3,0.9,90,0};
        printf("Movj to targets: {0,-0.3, 0.9, 90.0, 0.0}...\n");
        cartesianClient.movj(targets);
    }
    if (!_quiet) printf("[TaskGrabCan] success: end{run()}\n");
    return true;
}

/************************************************************************/
bool TaskGrabCan::close() {
    if (!_quiet) printf("[TaskGrabCan] success: begin{close()}\n");
    if (!_quiet) printf("[TaskGrabCan] success: end{close()}\n");
    return true;
}

