// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskGrabCan.hpp"

/************************************************************************/
bool TaskGrabCan::configure(yarp::os::Searchable& config) {
    return true;
}

/************************************************************************/
bool TaskGrabCan::run() {
    printf("[TaskGrabCan] succeess: begin{run()}\n");
    {
        double targets[5] = {0,-0.3,0.9,90,0};
        printf("Movj to targets: {0,-0.3, 0.9, 90.0, 0.0}...\n");
        cartesianClient.movj(targets);
    }
    printf("[TaskGrabCan] succeess: end{run()}\n");
    return true;
}

/************************************************************************/
bool TaskGrabCan::close() {
    return true;
}

