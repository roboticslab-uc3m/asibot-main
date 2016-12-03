// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * @ingroup asibot_examples_cpp
 *
 * \defgroup testTaskLib testTaskLib
 *
 * @brief This is an example of the use of TaskLib.
 *
 * @section testTaskLib_legal Legal
 *
 * Copyright: (C) 2013 Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * @section testTaskLib_build Building
\verbatim
cd repos/asibot-main/example/cpp
mkdir build; cd build; cmake ..
make -j3
\endverbatim
 *
 * @section testTaskLib_run Running
\verbatim
./testTaskLib
\endverbatim
 *
 */

#include <stdio.h>
#include <yarp/os/Property.h>

#include "TaskGrabCanSim.hpp"

int main(int argc, char *argv[]) {

    printf("testTaskLib checking for yarp network... ");
    fflush(stdout);
    Network yarp;
    if (!yarp.checkNetwork()) {
        printf("[fail]\ntestTaskLib found no yarp network (try running \"yarpserver &\"), bye!\n");
        return -1;
    } else printf("[ok]\n");

    TaskGrabCanSim task;
    Property options;
    options.put("robotAngle",30.0);
    task.configure(options);
    if(! task.run() ) fprintf(stderr,"[testTaskLib] failed: TaskGrabCan.run()\n");
    task.close();

    printf("Bye!\n");

    return 0;
}

