#! /usr/bin/env python

print "WARNING: only works with RaveBot in YARP yarpmods instead of in ASIBOT RlPlugins"

import yarp

yarp.Network.init()

if not yarp.Network.checkNetwork():
    print "[error] Please try running yarp server"
    quit()

options = yarp.Property()
options.put('device','ravebot')
dd = yarp.PolyDriver(options)

pos = dd.viewIPositionControl()
vel = dd.viewIVelocityControl()
enc = dd.viewIEncoders()
mode = dd.viewIControlMode()

axes = enc.getAxes()

for i in range(1,axes): mode.setPositionMode(i-1)

print "test positionMove(1,35)"
pos.positionMove(1,35)

print "test delay(5)"
yarp.Time.delay(5)

v = yarp.DVector(axes)
enc.getEncoders(v)
print v[2]

for i in range(1,axes): mode.setVelocityMode(i-1)
print "test velocityMove(0,10)"
vel.velocityMove(0,10)

print "test delay(5)"
yarp.Time.delay(5)

vel.velocityMove(0,0)

yarp.Network.fini()

