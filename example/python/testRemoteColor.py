#! /usr/bin/env python

##
# @ingroup asibot_examples_py
# \defgroup testRemoteColorPy testRemoteColor.py
#
# This example connects to a running \ref colorSegmentor or \ref colorDepth module using the C++ implementation
# of the \ref ColorClientLib library.
#
# @section testRemoteColorPy_legal Legal
#
# Copyright: (C) 2013 Universidad Carlos III de Madrid
#
# Author:  <a href="http://roboticslab.uc3m.es/roboticslab/persona.php?id_pers=72">Juan G. Victores</a>
#
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
#
# @section testRemoteColorPy_running Running (assuming correct installation)
#
#\verbatim
#python repos/asibot-main/example/python/testRemoteColor.py
#\endverbatim
#
# <hr>
#
# This file can be edited at repos/asibot-main/example/python/testRemoteColor.py
#

import yarp  # imports YARP
import asibot  # imports ASIBOT
import numpy as np  # imports numpy as np

yarp.Network.init()  # connect to YARP network

if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
    print '[fail] testRemoteColor found no yarp network (try running "yarpserver &"), bye!'
    quit()

color = asibot.ColorClient()  # create an instance of ColorClient
if color.open('/colorSegmentor',False) != True:  # use this line instead of next one for debuging purposes.
#if color.open('/colorSegmentor') != True:  # tries to connect. syntax: color.open('/prefix',quiet=True)
	print '[fail] Could not open ColorClient. Bye!'
	quit()

ok = True

size = color.size()

values = yarp.DVector()  # yarp.DVector() is actually a std::vector<double>
ok = color.get(values)  # Get coordinates of point 0 (timestamp) and put it in values.

if (ok == False) or (size == 0):
	print '[warning] Something failed!'

print 'contains',size,'points.'

print 'Number of coordinates per point (should remain static):',size

values = np.array(values)  # yes, this is legal!
print 'Values:',values

color.close()

print 'Bye!'

