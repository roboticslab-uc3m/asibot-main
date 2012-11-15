// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskCatchTool.h"

/************************************************************************/
TaskCatchTool::TaskCatchTool() { }

/************************************************************************/
bool TaskCatchTool::open(const ConstString& serverPrefix) {
    if (!cartesianClient.open("/ravebot"))  return false;
    ConstString clientStr(serverPrefix);
    clientStr += "/world/rpc:o";
    worldRpcClient.open(clientStr);
    ConstString serverStr(serverPrefix);
    serverStr += "/world";
    if (!worldRpcClient.addOutput(serverStr)) {
        fprintf(stderr,"[error] no world connection possible\n");
        return false;
    }
    printf("[success] Opened connection with world.\n");
    return true;
}

/************************************************************************/
bool TaskCatchTool::close() {
    cartesianClient.close();
    printf("worldRpcClient close...\n");
    worldRpcClient.interrupt();
    worldRpcClient.close();
    return true;
}

/************************************************************************/
bool TaskCatchTool::run(const ConstString a, const ConstString b, const ConstString c, const ConstString d) {
    printf("[success] TaskCatchTool::run(): begin.\n");
    {
        double targets[5] = {0,-0.3,0.9,90,0};
        printf("Movj to targets: {0,-0.3, 0.9, 90.0, 0.0}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    {
        double targets[5] = {0,-.7,.7,110,0};
        printf("Movj to targets: {0,-.7,.7,110,0}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    {
        double targets[5] = {0,-.72,0.33,170,0};
        printf("Movj to targets: {-.05,-.72,0.33,170,0}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    //
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addString("world");
    miOutput.addString("grab");
    miOutput.addString("obj");
    miOutput.addString("makeupTool");
    miOutput.addInt(1);
    worldRpcClient.write(miOutput, miInput);
    yarp::os::Time::delay(0.5);
    //
    {
        double targets[5] = {0,-.7,.7,110,0};
        printf("Movj to targets: {0,-.7,.7,110,0}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    {
        double targets[5] = {.07,-.2,0.7,90,-20};
        printf("Movj to targets: {.07,-.2,0.7,90,-20}...\n");
        cartesianClient.movj(targets);
    }
    printf("Wait...\n");
    cartesianClient.wait();
    //
    printf("[success] TaskCatchTool::run(): end.\n");
    return true;
}

/************************************************************************/

