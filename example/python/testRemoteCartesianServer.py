#! /usr/bin/env python

##
# @ingroup asibot_examples_py
#
# \defgroup testRemoteCartesianServerPy testRemoteCartesianServer.py
#
# This example connects to a running \ref cartesianServer module using the Python implementation
# of the \ref CartesianClient library to move in cartesian Space.
#
# <b>Legal</b> 
#
# Copyright: (C) 2012 Universidad Carlos III de Madrid
#
# Author: Juan G Victores
#
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
#
# <b>Installation</b>
#
# We basically just need to give the module running (execution) permissions. In Ubuntu/Linux:
#
#\verbatim
#chmod +x repos/asibot-main/example/python/testRemoteCartesianServer.py
#\endverbatim
#
# <b>Running</b> (assuming correct installation)
#
#\verbatim
#repos/asibot-main/example/python/testRemoteCartesianServer.py
#\endverbatim
#
# <b>Modify</b>
#
# This file can be edited at repos/asibot-main/example/python/testRemoteCartesianServer.py
#

print "WARNING: requires a running instance of cartesianServer"

import yarp  # imports YARP
yarp.Network.init()  # connect to YARP network
if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
    print "[error] Please try running yarp server"  # tell the user to start one with 'yarp server' if there isn't any
    quit()

from CartesianClient import CartesianClient  # imports the CartesianClient library

simCart = CartesianClient()  # make a cartesian controller object we call 'simCart'
simCart.open('/ravebot')  # make the 'simCart' establish connections

print "asking for inversion of [0.1, 0.0, 0.9, 90.0, 0.0]"
inversion = [0.1, 0.0, 0.9, 90.0, 0.0]
coords = []
res = simCart.inv(inversion,coords)
print coords

targets = [0.1, 0.0, 0.9, 90.0, 0.0]
print "Movj to targets: {0.1, 0.0, 0.9, 90.0, 0.0}..."
simCart.movj(targets)

print "Delaying 5 seconds..."
yarp.Time.delay(5)

print "Stopping..."
simCart.stop()

newtargets = [0.4, 0.0, 0.9, 90.0, 0.0]
print "Movl to newtargets: {0.4, 0.0, 0.9, 90.0, 0.0}..."
simCart.movl(newtargets)

print "Delaying 20 seconds..."
yarp.Time.delay(20)

print "asking for current position"
coords = []
res = simCart.stat(coords)
print coords

print "Bye!"

simCart.close()  # close the 'simCart' connections

yarp.Network.fini()  # disconnect from the YARP network


