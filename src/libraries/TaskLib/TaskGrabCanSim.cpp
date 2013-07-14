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
    double X_0_redCan[3] = {data->get(0).asDouble(), data->get(1).asDouble(), data->get(2).asDouble()};
    //if (!_quiet) printf("[TaskGrabCanSim] Got info on redCan: %s\n",bIn.toString().c_str());
    if (!_quiet) printf("[TaskGrabCanSim] X_0_redCan: %f %f %f.\n",X_0_redCan[0],X_0_redCan[1],X_0_redCan[2]);
    //
    bOut.clear(); bIn.clear();
    bOut.addString("world");
    bOut.addString("whereis");
    bOut.addString("tcp");
    _worldRpcClient.write(bOut, bIn);
    data = bIn.get(0).asList();
    double X_0_tcp[3] = {data->get(0).asDouble(), data->get(1).asDouble(), data->get(2).asDouble()};
    if (!_quiet) printf("[TaskGrabCanSim] X_0_tcp: %f %f %f.\n",X_0_tcp[0],X_0_tcp[1],X_0_tcp[2]);
    //
    double X_base_tcp[5];
    _cartesianClient.stat(X_base_tcp);
    if (!_quiet) printf("[TaskGrabCanSim] X_base_tcp: %f %f %f.\n",X_base_tcp[0],X_base_tcp[1],X_base_tcp[2]);
    //
    double X_0_base[3] = {X_0_tcp[0]-X_base_tcp[0], X_0_tcp[1]-X_base_tcp[1], X_0_tcp[2]-X_base_tcp[2]};
    if (!_quiet) printf("[TaskGrabCanSim] X_0_base: %f %f %f.\n",X_0_base[0],X_0_base[1],X_0_base[2]);
    //
    yarp::sig::Matrix H_0_base = rotZ(_robotAngle);
    H_0_base.resize(4,4);
    H_0_base(0,3)=X_0_base[0];
    H_0_base(1,3)=X_0_base[1];
    H_0_base(2,3)=X_0_base[2];
    H_0_base(3,3)=1;
    if (!_quiet) printf("*** H_0_base *** \n(%s)\n\n", H_0_base.toString().c_str());
    //
    yarp::sig::Matrix H_base_0 = pinv(H_0_base);
    if (!_quiet) printf("*** H_base_0 *** \n(%s)\n\n", H_base_0.toString().c_str());
    //
    yarp::sig::Matrix H_0_redCan(4,4);
    H_0_redCan.eye();
    H_0_redCan(0,3)=X_0_redCan[0];
    H_0_redCan(1,3)=X_0_redCan[1];
    H_0_redCan(2,3)=X_0_redCan[2]+.05;  // +.05 to avoid table
    if (!_quiet) printf("*** H_0_redCan *** \n(%s)\n\n", H_0_redCan.toString().c_str());
    //
    yarp::sig::Matrix H_base_redCan = H_base_0 * H_0_redCan;
    if (!_quiet) printf("*** H_base_redCan *** \n(%s)\n\n", H_base_redCan.toString().c_str());
    
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

