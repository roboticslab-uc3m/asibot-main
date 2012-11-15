// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskMakeUp.h"

/************************************************************************/
TaskMakeUp::TaskMakeUp() { }

/************************************************************************/
bool TaskMakeUp::open(const ConstString& serverPrefix) {
    if (!cartesianClient.open("/ravebot"))  return false;
    return true;
}

/************************************************************************/
bool TaskMakeUp::close() {
    cartesianClient.close();
    return true;
}

/************************************************************************/
bool TaskMakeUp::run(const ConstString a, const ConstString b, const ConstString c, const ConstString d, const ConstString e, const ConstString f, const ConstString g) {
    printf("[success] TaskMakeUp::run(): begin.\n");
    for(int i=0;i<3;i++) {
        {
            double targets[5] = {0.29417,-0.207409,0.577277,100.237032,150.187567};
            printf("Movj to targets: {0.29417,-0.207409,0.577277,100.237032,150.187567}...\n");
            cartesianClient.movj(targets);
        }
        printf("Wait...\n");
        cartesianClient.wait();
        {
            double targets[5] = {0.25535,-0.253675,0.577277,100.237032,150.187567};
            printf("Movj to targets: {0.25535,-0.253675,0.577277,100.237032,150.187567}...\n");
            cartesianClient.movj(targets);
        }
        printf("Wait...\n");
        cartesianClient.wait();
    }
    //
    /*{
        double targets[5] = {0.262,-0.134,0.638,90,160};
        printf("Movj to targets: {0.262,-0.134,0.638,90,160}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();*/
    {
        double targets[5] = {0,-0.1,0.9,90,160};
        printf("Movj to targets: {0,-0.1,0.9,90,160}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    //
    printf("[success] TaskMakeUp::run(): end.\n");
    return true;
}

/************************************************************************/

