import yarp
yarp.Network.init()
if yarp.Network.checkNetwork() != True:
    print "[error] Please try running yarp server\n"
    quit()
options = yarp.Property()
options.put('local','/local')
options.put('remote','/ravebot')
options.put('device','remote_controlboard')
dd = yarp.PolyDriver(options)

pos = dd.viewIPositionControl()
pos.setPositionMode()
pos.positionMove(1,-45)

enc = dd.viewIEncoders()
v = yarp.DVector(enc.getAxes())
enc.getEncoders(v)
v[2]

vel = dd.viewIVelocityControl()
vel.setVelocityMode()
vel.velocityMove(0,40)

options2 = yarp.Property()
options2.put('device','cartesiankontroller')
dd2 = yarp.PolyDriver()
dd2.open(options2) 
cart = dd2.viewICartesianControl()

v = yarp.Vector()
v.push_back(0.0)
v.push_back(0.0)
v.push_back(1.0)
cart.goToPositionSync(v,4.0)
cart.waitMotionDone(0.1,4.0)

yarp.Network.fini()

