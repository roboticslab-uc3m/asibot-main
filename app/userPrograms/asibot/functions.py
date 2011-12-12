#!/usr/bin/python

# Get some YARP networking
import yarp
from yarp import Time

class DataProcessor(yarp.PortReader):
    qflag = None
    def read(self,connection):
        print("in DataProcessor.read")
        if not(connection.isValid()):
            print("Connection shutting down")
            return False
        btin = yarp.Bottle()
        bout = yarp.Bottle()
        print("Trying to read from connection")
        ok = btin.read(connection)
        if not(ok):
            print("Failed to read input")
            return False
        print("Received: %s"%btin.toString())
        if btin.get(0).asVocab() == yarp.Vocab.encode("quit"):
            print "I really have to go"
            self.qflag = True
            bout.addVocab(yarp.Vocab.encode("ok"))
        else:
            bout.addVocab(yarp.Vocab.encode("fail"))
        print("Sending: %s"%bout.toString())
        writer = connection.getWriter()
        if writer==None:
            print("No one to reply to")
            exit()
            return True
        return bout.write(writer)


class robtarget:
    # __init__ is the constructor
    def __init__(self, a, b, c, d, e):
        self.v = yarp.DVector(5)
        self.v[0] = a
        self.v[1] = b
        self.v[2] = c
        self.v[3] = d
        self.v[4] = e

class asibot:
    pos = None
    dd = None
    pquit = None
    r = None
    # __init__ is the constructor
    def __init__(self, str):
        yarp.Network.init()
        if yarp.Network.checkNetwork() != True:
            print "[error] Please try running yarp server\n"
            quit()
        options = yarp.Property()
        options.put('local','/userProgram')
        mstr = "/" + str
        options.put('remote',mstr)
        options.put('device','remote_controlboard')
        self.dd = yarp.PolyDriver()
        if not self.dd.open(options):
            print "[warning] " + str + " device not available"
            print "[warning] Please try running $ASIBOT_DIR/bin/test_" + str + "\n"
        self.pos = self.dd.viewIPositionControl()
        self.pquit = yarp.Port()
        self.r = DataProcessor()
        self.pquit.setReader(self.r)
        self.pquit.open("/userProgram/quit")


    def MoveL(self, target):
        print str(target.v[0]) + str(target.v[1]) + str(target.v[2]) + str(target.v[3]) + str(target.v[4])
        self.r.qflag = False
        self.pos.positionMove(target.v)
        for thejoint in range(self.pos.getAxes()):
            motionDone = yarp.BVector(1)
            motionDone[0] = False
            while not motionDone[0] and (self.r.qflag != True):
                self.pos.checkMotionDone(thejoint,motionDone)
                Time.delay(0.1)
            if self.r.qflag == True:
                self.pos.stop()
                quit()
            print "Joint q" + str(thejoint) + " reached target."

#        b = yarp.BVector(self.pos.getAxes())
#        self.pos.checkMotionDone(b)
#        if (b[0]==1):
#            print "0 Reached.\n"
#        else:
#            print "0 Not yet.\n"
#        if (b[1]==1):
#            print "1 Reached.\n"
#        else:
#            print "1 Not yet.\n"


    def finiRobot():
        yarp.Network.fini()
        return

#cart.goToPositionSync(v,4.0)
#cart.waitMotionDone(0.1,4.0)
#enc = dd.viewIEncoders()
#pos.setPositionMode()
#pos.positionMove(1,-45)
#enc.getEncoders(v)
#v[2]
#v = yarp.DVector(enc.getAxes())


