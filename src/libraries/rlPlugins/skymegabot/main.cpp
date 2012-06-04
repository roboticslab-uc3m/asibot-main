// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

// Author: Juan G Victores 2012 @ ASROB @ UC3M w/ LGPL +3.0

#include "SkymegaBot.h"

int main(int argc, char *argv[]) {
    SkymegaBot skymegabot;
    skymegabot.open();
    while(1){
        for(int i = 1500; i<2200; i+=200) {
            printf("[main] velocityMove(0, %d)\n",i);
            skymegabot.velocityMove(0, i);
            skymegabot.velocityMove(1, i);
            sleep(1);
        }
    }
    return 0;
}
