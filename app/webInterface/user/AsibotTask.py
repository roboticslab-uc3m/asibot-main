import yarp
from yarp import Time
yarp.Network.init()
if yarp.Network.checkNetwork() != True:
    print "[error] Please try running yarp server"
    quit()

import sys, os
asibot_root = os.environ['ASIBOT_ROOT']
sys.path.append(os.path.join(asibot_root,'example/python'))
from CartesianClient import CartesianClient  # imports the CartesianClient library

