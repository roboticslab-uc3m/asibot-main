import yarp
from yarp import Time
yarp.Network.init()
if yarp.Network.checkNetwork() != True:
    print "[error] Please try running yarp server"
    quit()

from CartesianClient import CartesianClient  # imports the CartesianClient library

