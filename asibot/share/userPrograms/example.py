#!/usr/bin/python
import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),'asibot'))
from functions import *


########### Program starts here ###########

P1 = robtarget(0.0,0.0,0.0,0.0,0.0)
P2 = robtarget(44.5,0.0,0.0,0.0,0.0)
P3 = robtarget(44.5,-9.6,-15.0,-9.6,0.0)

myrobot = asibot("ravebot")
myrobot.MoveL(P1)
myrobot.MoveL(P2)
myrobot.MoveL(P3)
myrobot.MoveL(P2)
myrobot.MoveL(P3)
myrobot.MoveL(P2)
myrobot.MoveL(P1)

print "DONE"



