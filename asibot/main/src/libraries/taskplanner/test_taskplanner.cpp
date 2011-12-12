// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Author: Juan G. Victores
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 * Description: This cpp is more like a network wrapper
 *
 */

#include "taskplanner.h"

int main(int argc, char *argv[]) {
    printf("*** WARNING: This program is a stub ***\n");

    Network yarp;
    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    RpcServer portGoal; // Should actually be RpcServer
    Port portPlan; // Should actually be rpc
    portGoal.open("/taskplanner/goal:i");
    portPlan.open("/taskplanner/plan:o");
    yarp.connect("/taskplanner/plan:o","/taskexecuter/plan:i");
    while (true) {
        printf("\nWaiting for input... ");
        fflush(stdout);
        Bottle input;
        portGoal.read(input,true);
        if(input.size()==2) {
            printf("got %s\n",input.toString().c_str());
            Bottle* goal = input.get(1).asList();
            ConstString who(goal->get(0).asString());
            ConstString act(goal->get(1).asString());
            ConstString obj(goal->get(2).asString());
            printf("<who>: %s\n", who.c_str());
            printf("<act>: %s\n", act.c_str());
            printf("<obj>: %s\n", obj.c_str());
            Bottle thePlan;
            bool ret = planTasks(who,act,obj,thePlan);
            // Now see what to do with the plan
            ConstString mode(input.get(0).asString());
            if(mode == ConstString("plan")) {
                portGoal.reply(thePlan);
            } else if (mode == ConstString("run")) {
                portPlan.write(thePlan);
                Bottle response;
                response.addString("sent to planexecuter!");
                portGoal.reply(response);
            } else {
                Bottle response;
                response.addString("please input as in: plan/run (<who> <act> <obj>)");
                portGoal.reply(response);
            }
        } else {
            Bottle response;
            response.addString("please input as in: plan/run (<who> <act> <obj>)");
            portGoal.reply(response);
        }
    }
    return 0;
}

        /*          printf("Real, simulated, or both? (real/sim/both)\n");
            portplan.read(input);
            printf("Step-by-step, or automatic? (step/auto)\n");
            portplan.read(input);
            printf("Confirm... (okay)\n");
            portplan.read(input);*/

