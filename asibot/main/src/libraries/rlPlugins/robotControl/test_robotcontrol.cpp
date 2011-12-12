/*
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Copyright: (C) 2010 RobotCub Consortium
 * Author: Juan G. Victores, based on the work of Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <stdio.h>
#include <yarp/os/Log.h>
#include <yarp/os/Property.h>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/Terminator.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ServiceInterfaces.h>
#include <yarp/dev/Drivers.h>

#include <yarp/dev/CartesianControl.h>
#include <yarp/sig/Vector.h>

#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_unistd.h> 
#include <ace/OS_NS_signal.h>
#include <ace/Vector_T.h>

#include "CartesianKontroller2.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_PLUGINS(rlYarpmods);

int main(int argc, char *argv[]) {

  hola();

  Network yarp;
  if (!Network::checkNetwork()) {
    printf("Please start a yarp name server first\n");
    return(-1);
  }

  Property config;
  config.fromCommand(argc, argv);

  YARP_REGISTER_PLUGINS(rlYarpmods);

  // check our device can be wrapped in the controlboard network wrapper
  // and accessed remotely
  printf("================================\n");
  printf("check our device can be accessed\n");

  Property options;
  options.put("device","robotcontrol");
  // TODO: Pass more options such as jmc name, which shoul also be interpreted at lib side
  PolyDriver dd(options);

  if (!dd.isValid()) {
    printf("[error] Server instantiation not worked.\n\n");
    printf("Be sure CMake \"ENABLE_cartesianBlaBlaBla\" variable is set \"ON\"\n");
    printf("\"SKIP_cartesianBlaBlaBla is set\" --> should be --> \"ENABLE_cartesianBlaBlaBla is set\"\n\n");
    printf("You should also make sure your remote_controlboard is on.\n\n");
    dd.close();
    return -1;
  }

  ICartesianControl *cart;
  dd.view(cart);
  yarp::sig::Vector myx;
  yarp::sig::Vector myo;

  cart->getPose(myx,myo);
printf("mHERE\n");
  yarp::sig::Vector xd;
  xd.resize(3);
  xd[0] = 0.1;
  xd[1] = 0.1;
  xd[2] = 1.0;
  printf("mHERE1\n");
  cart->goToPositionSync(xd);
  printf("mHERE2\n");
  Time::delay(1);
  dd.close();
  return 0;
}



