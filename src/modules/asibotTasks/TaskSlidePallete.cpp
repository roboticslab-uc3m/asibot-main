// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskSlidePallete.h"

/************************************************************************/
TaskSlidePallete::TaskSlidePallete() { }

/************************************************************************/
bool TaskSlidePallete::open(const ConstString& serverPrefix) {
    Property options;
    options.put("device","remote_controlboard");
    options.put("remote","/ravebot");
    options.put("local","/local");
    dd.open(options);
    if(!dd.isValid()) {
      printf("Robot device not available.\n");
	  dd.close();
      Network::fini();
      return 1;
    }
    printf("[success] Opened connection with robot.\n");
    bool ok = dd.view(pos) && dd.view(mode);
    if (!ok) {
        printf("[warning] Problems acquiring robot interface\n");
        return false;
    } else printf("[success] Acquired robot interface\n");

    return true;
}

/************************************************************************/
bool TaskSlidePallete::close() {
    dd.close();
    printf("sent robot device close...\n");
    return true;
}

/************************************************************************/
bool TaskSlidePallete::run(const ConstString a, const ConstString b, const ConstString c, const ConstString d, const ConstString e, const ConstString f) {
    printf("[success] TaskSlidePallete::run(): begin.\n");
    int axes;
    pos->getAxes(&axes);
    for (unsigned int i = 0; i < axes; i++)
        mode->setPositionMode(i);
    pos->positionMove(4,150);
    bool done = false;
    while(!done) {
        pos->checkMotionDone(&done);
        printf(".");
        fflush(stdout);
        Time::delay(0.5);
    }
    printf("\n");
    printf("[success] TaskSlidePallete::run(): end.\n");
    return true;
}

/************************************************************************/

