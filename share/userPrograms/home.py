#!/usr/bin/python
import sys, os
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)),'asibot'))
from functions import *


########### Program starts here ###########

P1 = robtarget(0.0,0.0,0.0,0.0,0.0)

myrobot = asibot("ravebot")
myrobot.MoveL(P1)

