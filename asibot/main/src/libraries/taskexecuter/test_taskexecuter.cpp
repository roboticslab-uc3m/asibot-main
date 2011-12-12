// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Author: Juan G. Victores
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 * Description: For now it's just a parser for LISP-like Bottles. This is because
 * LISP-like nomenclature is used in Automated Planning AND in YARP Bottles. Now we
 * need to make it link the semantic level to the functional level!!!
 */

#include "taskexecuter.h"
#include "yarp/os/Run.h"

int main(int argc, char *argv[]) {
    printf("*** WARNING: This program is a stub ***\n");

    Network yarp;
    if (!Network::checkNetwork()) {
        printf("Please start a yarp name server first\n");
        return(-1);
    }

    Port portplan;
    portplan.open("/taskexecuter/plan:i");
    while (true) {
        printf("\nWaiting for input... ");
        fflush(stdout);
        Bottle input;
        portplan.read(input);
        printf("got %s\n",input.toString().c_str());
        for (int i=0; i<input.size(); i++) {
            Bottle* instruction = input.get(i).asList();
            printf("instruction<%d>: %s\n",i,instruction->toString().c_str());
            for (int j=0; j<instruction->size(); j++) {
               printf("<one:%d,%d>: %s\n",i,j,instruction->get(j).toString().c_str());
            }
            printf("Going to execute instruction: %d.\n",i);
            ConstString machine("local");
            Property options;
            options.put("name",instruction->get(0).toString());
            ConstString mytag("prog1");
            int ret = yarp::os::Run::start(machine,options,mytag);
            printf("Done to executing instruction: %d, status: %d.\n",i,ret);
        }
    }
    return 0;
}

