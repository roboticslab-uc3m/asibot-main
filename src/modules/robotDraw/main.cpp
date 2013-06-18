// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * @ingroup asibot_modules
 * \defgroup robotDraw robotDraw
 *
 * @brief robotDraw opens a YARP rpc port to recieve "draw p0x p0y p1x p1y ... pNx pNy" commands.
 *
 * robotDraw opens a YARP rpc port to recieve "draw p0x p0y p1x p1y ... pNx pNy" commands
 * (i.e. "draw 0 0 .1 0 .1 .1 0 .1 0 0"), and draw the points on a plane. It connects to
 * a running \ref cartesianServer module using the C++ implementation of the \ref CartesianClientLib
 * library. Inherited from (original development of) the XGNITIVE repository.
 *
 * @section robotDraw_legal Legal
 *
 * Copyright: (C) 2013 Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>,
 * <a href="http://www.mendeley.com/profiles/santiago-morante-cendrero/">Santiago Morante</a>
 *
 * CopyPolicy: This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 3.0 or later
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>

#include "CartesianClient.h"

#define DEFAULT_ROBOT "/ravebot"
#define DEFAULT_HEIGHT 0.4;

#define VOCAB_DRAW VOCAB('d','r','a','w')

using namespace yarp::os;
using namespace yarp::dev;

int main(int argc, char *argv[]) {

    Network yarp;
    printf("Run \"%s --help\" for options.\n",argv[0]);
    printf("%s checking for yarp network... ",argv[0]);
    fflush(stdout);
    if (!yarp.checkNetwork()) {
        fprintf(stderr,"[fail]\n%s found no yarp network (try running \"yarpserver &\"), bye!\n",argv[0]);
        return -1;
    } else printf("[ok]\n");

    ConstString robotName(DEFAULT_ROBOT);
    double height = DEFAULT_HEIGHT;

    Property options;
    options.fromCommand(argc, argv);
    printf("--------------------------------------------------------------\n");
    if (options.check("help")) {
        printf("robotDraw options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--robot (default: \"%s\")\n",robotName.c_str());
        printf("\t--height (default: \"%f\")\n",height);
    }
    if (options.check("robot")) robotName = options.find("robot").asString();
    if (options.check("height")) height = options.find("height").asDouble();
    printf("robotDraw using robot: %s, height.\n",robotName.c_str(),height);

    printf("--------------------------------------------------------------\n");
    if(options.check("help")) {
        ::exit(1);
    }

    CartesianClient cartesianClient;
    while(!cartesianClient.open(robotName)) {
        printf("Waiting for cartesianServer...\n");
        Time::delay(1);
    }

    RpcClient worldRpcClient;
    ConstString worldRpcRemoteName(robotName);
    worldRpcRemoteName += "/world";
    ConstString portNamePrefix("/robotDraw");
    ConstString worldRpcClientName = portNamePrefix + worldRpcRemoteName;
    if(!worldRpcClient.open(worldRpcClientName)) {
        fprintf(stderr,"[fail] Bad worldRpcClient, bye!\n");
        return -1;
    }
    yarp.connect(worldRpcClientName,worldRpcRemoteName);

    RpcServer pointsRpcServer;
    ConstString pointsRpcServerName(portNamePrefix);
    pointsRpcServerName += "/rpc:i";
    if(!pointsRpcServer.open(pointsRpcServerName)) {
        fprintf(stderr,"[fail] pointsRpcServer\n");
        return -1;
    }

    double home[5] = {0, -0.1, .9, 90.0, 0.0};
    printf("[movj] To home.\n");           
    cartesianClient.movj(home);
    Time::delay(.1);
    cartesianClient.wait();

    double targets[5] = {0, -0.45, height, 180.0, 0.0};

    while (1) {
        printf("[success] %s on BLOCKING WAIT for draw request,\n", pointsRpcServerName.c_str());
        printf("usage: draw r g b p0x p0y p1x p1y ... pNx pNy\n");
        printf("example: draw 1 1 1 0 0 .1 0 .1 .1 0 .1 0 0\n");

        Bottle bIn, bOut;
        pointsRpcServer.read(bIn,true);
        printf("[pointsRpcServer] Got %s\n", bIn.toString().c_str());                
        bOut.clear();
        bOut.addVocab(VOCAB_OK);
        pointsRpcServer.reply(bOut);  // send reply

        double aprox0[5] = {targets[0]+bIn.get(5).asDouble(), targets[1]+bIn.get(6).asDouble(),
            targets[2]+.1, targets[3], targets[4]};
        printf("[movj] To aprox0.\n");                
        cartesianClient.movj(aprox0);
        Time::delay(.1);
        cartesianClient.wait();

        double p0[5] = {targets[0]+bIn.get(5).asDouble(), targets[1]+bIn.get(6).asDouble(),
            targets[2], targets[3], targets[4]};
        printf("[movl] To p0.\n");          
        cartesianClient.movl(p0);
        Time::delay(.1);
        cartesianClient.wait();

        Bottle miOutput, miInput;
        miOutput.clear();
        miOutput.addString("world");
        miOutput.addString("draw");
        miOutput.addInt(1);  // ON
        miOutput.addDouble(bIn.get(1).asDouble());  // Radius: 0.005
        miOutput.addDouble(bIn.get(2).asDouble());  // R: 0.0980392
        miOutput.addDouble(bIn.get(3).asDouble());  // G: 0.705882
        miOutput.addDouble(bIn.get(4).asDouble());  // B: 0.686274
        worldRpcClient.write(miOutput, miInput);
        Time::delay(.1);

//        for (int i=3; i<(bIn.size());i=i+2) {
        for (int i=7; i<(bIn.size());i=i+2) {
            double tmpTargets[5];
            tmpTargets[0] = targets[0] + bIn.get(i).asDouble();
            tmpTargets[1] = targets[1] + bIn.get(i+1).asDouble();
            tmpTargets[2] = targets[2];
            tmpTargets[3] = targets[3];
            tmpTargets[4] = targets[4];
            cartesianClient.movl(tmpTargets);
            Time::delay(.1);
            cartesianClient.wait();
        }

        miOutput.clear();
        miOutput.addString("world");
        miOutput.addString("draw");
        miOutput.addInt(0);
        worldRpcClient.write(miOutput, miInput);
        Time::delay(.1);

        double aproxN[5] = {targets[0]+bIn.get(bIn.size()-2).asDouble(), targets[1]+bIn.get(bIn.size()-1).asDouble(),
            height+.1, targets[3], targets[4]};
        printf("[movl] To aproxN.\n");                
        cartesianClient.movl(aproxN);
        Time::delay(.1);
        cartesianClient.wait();

        printf("[movj] To home.\n");           
        cartesianClient.movj(home);
        Time::delay(.1);
        cartesianClient.wait();

    }

    printf("Bye!\n");
    cartesianClient.close();

    return 0;
}


