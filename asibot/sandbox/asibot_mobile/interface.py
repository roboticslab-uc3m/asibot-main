#! /usr/bin/env python

"""
* Copyright: (C) 2011 Universidad Carlos III de Madrid
* Authors: Juan G Victores & Santiago Morante
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
"""

# Import CherryPy global namespace (server)
import cherrypy

# Get some YARP networking
import yarp

class webAsibot:

    asibot_state = 0
    sim_state = 0
    teachfile = "None"
    jointvals = [0.0, 0.0, 0.0, 0.0, 0.0]

    def index(self):
        page = open("mobile/index.html").read()
        return page
    index.exposed = True

    def docking(self, action=None):
    	print action
    	
#    	if action == "dock":
#			do whatever
#    	if action == "undock":
#			do whatever
    	
        page = open("mobile/docking.html").read()
        return page
    docking.exposed = True


    def mode(self, option=None):
    	print option
    	
        page = open("mobile/mode.html").read()

        if option == "asibot_con":
            yarp.Network.connect("/asibot_web","/can_q")
            yarp.Network.connect("/file_q_o","/can_q")
            # set priority feedback channel (q, file)
            yarp.Network.connect("/can_q","/asibot_web")
            yarp.Network.connect("/can_q","/file_cmd")
            if self.sim_state == 1:
                yarp.Network.disconnect("/rave_q","/asibot_web")
                yarp.Network.disconnect("/rave_q","/file_cmd")
            # no real kin because joint velocity not implemented in real
            self.asibot_state = 1
        if option == "asibot_dis":
            yarp.Network.disconnect("/asibot_web","/can_q")
            yarp.Network.disconnect("/file_q_o","/can_q")
            # undo the set priority feedback channel (q, file)
            yarp.Network.disconnect("/can_q","/asibot_web")
            yarp.Network.disconnect("/can_q","/file_cmd")
            self.asibot_state = 0

        if option == "sim_con":
            yarp.Network.connect("/asibot_web","/rave_q")
            yarp.Network.connect("/file_q_o","/rave_q")
            # feedback on kinematics
            yarp.Network.connect("/kdl_q","/rave_q")
            yarp.Network.connect("/rave_q","/kdl_q")
            # secondary feedback channel (q, file)
            if self.asibot_state == 0:
                yarp.Network.connect("/rave_q","/asibot_web")
                yarp.Network.connect("/rave_q","/file_cmd")
            self.sim_state = 1
        if option == "sim_dis":
            yarp.Network.disconnect("/asibot_web","/rave_q")
            yarp.Network.disconnect("/file_q_o","/rave_q")
            # feedback on kinematics
            yarp.Network.disconnect("/kdl_q","/rave_q")
            yarp.Network.disconnect("/rave_q","/kdl_q")
            # secondary feedback channel (q, file)
            yarp.Network.disconnect("/rave_q","/asibot_web")
            self.sim_state = 0

        if self.asibot_state == 1:
            page = page.replace('ASIBOTSTATE','asibot_con')
            page = page.replace('SETASIBOT','asibot_dis')
        else:
            page = page.replace('ASIBOTSTATE','asibot_dis')
            page = page.replace('SETASIBOT','asibot_con')

        if self.sim_state == 1:
            page = page.replace('SIMSTATE','sim_con')
            page = page.replace('SETSIM','sim_dis')
        else:
            page = page.replace('SIMSTATE','sim_dis')
            page = page.replace('SETSIM','sim_con')

        return page
    mode.exposed = True

    def articular(self, joint=-1, movement=None, one=0, two=0, three=0, four=0, five=0, stop=0):
        if one == '': one = 0
        if two == '': two = 0
        if three == '': three = 0
        if four == '': four = 0
        if five == '': five = 0
        print joint
        print movement
        print one
        print two
        print three
        print four
        print five
        print stop
        
# first send kind of movement, then the doubles (floats in python), last speed percentage
        botella=yarp.Bottle()
        if movement == "left":
            botella.addInt(1) # relative
            for iteration in range(1,6):
                if int(joint) == iteration:
                    botella.addDouble(-5.0)
                else:
                    botella.addDouble(0.0)
            botella.addDouble(60.0)
            if int(joint) != -1:
                p.write(botella)
        elif movement == "right":
            botella.addInt(1) # relative
            for iteration in range(1,6):
                if int(joint) == iteration:
                    botella.addDouble(5.0)
                else:
                    botella.addDouble(0.0)
            botella.addDouble(60.0)
            if int(joint) != -1:
                p.write(botella)
        elif movement == "absolute":
            botella.addInt(2)
            botella.addDouble(float(one))
            botella.addDouble(float(two))
            botella.addDouble(float(three))
            botella.addDouble(float(four))
            botella.addDouble(float(five))
            botella.addDouble(60.0) # speed
            p.write(botella)
        elif movement == "relative":
            botella.addInt(1)
            botella.addDouble(float(one))
            botella.addDouble(float(two))
            botella.addDouble(float(three))
            botella.addDouble(float(four))
            botella.addDouble(float(five))
            botella.addDouble(60.0) # speed
            p.write(botella)  
        elif int(stop) == 1:
            botella.addInt(-1)    
            p.write(botella) 

        page = open("mobile/articular.html").read()
        return page
    articular.exposed = True    

    def cartesian(self, stop=0):
    	print stop
    	
    	if int(stop) == 1:
            botella=yarp.Bottle()
            botella.clear()
            botella.addInt(-1)
            px.write(botella)
    		
        page = open("mobile/cartesian.html").read()
        return page
    cartesian.exposed = True


    def joypad(self, axis=None, origin=None, movement=None, stop=0): # of cartesian tab
    	print axis
    	print origin
    	print movement
    	print stop

    	if int(stop) == 1:
            botella=yarp.Bottle()
            botella.clear()
            botella.addInt(-1)
            px.write(botella)
    	
    	if axis != "None" and origin != "None" and movement != "None":
            botella=yarp.Bottle()
            if origin == "base":
                botella.addInt(2) # rel base
            elif origin == "tool":
                botella.addInt(3) # rel tool

            if movement == "left":
                incrx = -0.1
                incro = -5.0
            elif movement == "right":
                incrx = 0.1
                incro = 5.0
            for theaxis in ["x","y","z"]:
                if axis == theaxis:
                    botella.addString(theaxis)
                    botella.addDouble(incrx)
            for theaxis in ["roll","pitch","yaw"]:
                if axis == theaxis:
                    botella.addString(theaxis)
                    botella.addDouble(incro)

            px.write(botella) # already have good conditions

        page = open("mobile/joypad.html").read()
        return page
    joypad.exposed = True

    def numeric(self, origin=None, x=0, y=0, z=0, roll=0, pitch=0, yaw=0, stop=0):
    	print origin
    	print x
    	print y
    	print z
    	print roll
    	print pitch
    	print yaw
    	print stop

    	if int(stop) == 1:
            botella=yarp.Bottle()
            botella.clear()
            botella.addInt(-1)
            px.write(botella)

    	if origin != "None" :
            botella=yarp.Bottle()
            if origin == "abs_base":
                botella.addInt(1)
            if origin == "rel_base":
                botella.addInt(2)
            if origin == "tool":
                botella.addInt(3)
            if x != '':
                botella.addString("x")
                botella.addDouble(float(x))
            if y != '':
                botella.addString("y")
                botella.addDouble(float(y))
            if z != '':
                botella.addString("z")
                botella.addDouble(float(z))
            if roll != '':
                botella.addString("roll")
                botella.addDouble(float(roll))
            if pitch != '':
                botella.addString("pitch")
                botella.addDouble(float(pitch))
            if yaw != '':
                botella.addString("yaw")
                botella.addDouble(float(yaw))

            px.write(botella) # if origin != None

        page = open("mobile/numeric.html").read()
        return page
    numeric.exposed = True


    def command(self, order=None):
    	print order
    	if order != None:
    		botella=yarp.Bottle()
    		botella.addString(order)
    		p.write(botella)

        page = open("mobile/command.html").read()
        return page
    command.exposed = True

    def speech(self, word=None):
    	print word
    	action=None
    	
    	if word == "beber agua":
    		action="beber"
    		
    	if word == "lavar cara":
    		action="lavar"
    		
    	if word == "lavar dientes":
    		action="dientes"
    		
    	if word == "coger lata":
    		action="lata"
    		
    	if word == "para":
    		action="stop"

       	print action

        if action != None:
        #  here the instruction to execute actionwhatever.mats
            tarea = yarp.Bottle()
            tarea.clear()
            if action == 'stop':
                tarea.addInt(-1)
                pt.write(tarea)
            else:
                tarea.addString(str(action))
                pt.write(tarea)

        page = open("mobile/command.html").read()
        return page
    speech.exposed = True

    def create(self, nfile=None):
        if nfile != None:
            newfile = "mats/" + nfile + ".mats"
            if os.path.isfile(newfile):
                print "already exists"
            else:
                NFILE = open(newfile,"w")
                NFILE.close()
                print "created " + nfile

# need this repeated code, should pass to function...
        ficheroscargados = ''
        matsfiles = os.listdir('mats')
        for infile in matsfiles:
            if (infile.split('.')[1] == "mats") & (infile.split('.')[0] != "home"):
                ficheroscargados += '<option value="' + infile.split('.')[0] + '">' + infile.split('.')[0] + '</option>'

        page = open("mobile/teach.html").read()
        page = page.replace('<CARGARFICHEROS>',ficheroscargados)
        page = page.replace('FNAME',self.teachfile)
        return page
    create.exposed = True

    def teach(self, tfile=None, record=None):

        print file
        print record
        if tfile != None:
            self.teachfile = tfile
        
# here would come the explorer implementation 
        ficheroscargados = ''
        matsfiles = os.listdir('mats')
        for infile in matsfiles:
            if (infile.split('.')[1] == "mats") & (infile.split('.')[0] != "home"):
                ficheroscargados += '<option value="' + infile.split('.')[0] + '">' + infile.split('.')[0] + '</option>'

        page = open("mobile/teach.html").read()
        
# here would come the record button implementation 
        if record != None:
            editfile = "mats/" + self.teachfile + ".mats"
            if (record == "record") & (os.path.isfile(editfile)):
                EFILE = open(editfile,"a")
                mystr = "MOVAA,"
                for i in range(len(self.jointvals)):
                    mystr += str(self.jointvals[i]) + ","
                mystr += "60.0\n"
                EFILE.write(mystr)
                EFILE.close()
            if record == "update":
                botella = yarp.Bottle()
                rbotella = yarp.Bottle()
                botella.addInt(0)
                p.write(botella)
                p.read(rbotella)
                if rbotella != "":
                    self.jointvals[0] = float(rbotella.get(2).asDouble())
                    self.jointvals[1] = float(rbotella.get(4).asDouble())
                    self.jointvals[2] = float(rbotella.get(6).asDouble())
                    self.jointvals[3] = float(rbotella.get(8).asDouble())
                    self.jointvals[4] = float(rbotella.get(10).asDouble())
                    mystr = ""
                    for i in range(len(self.jointvals)):
                        mystr += str(self.jointvals[i]) + " "
                    page = page.replace('JOINTVALS', mystr)

        page = page.replace('<CARGARFICHEROS>',ficheroscargados)
        page = page.replace('FNAME',self.teachfile)
        return page
    teach.exposed = True

    def predefined(self, action=None):

        tareascargadas = ''
        matsfiles = os.listdir('mats')
        aleftflag = 1
        for infile in matsfiles:
            if (infile.split('.')[1] == "mats") & (infile.split('.')[0] != "home"):
                tareascargadas += '<button  name="action" type="submit" value="' + infile.split('.')[0] + '"><img src="images/' + infile.split('.')[0] + '.jpg" width="70" height="50" alt="Please upload jpg for ' + infile.split('.')[0] + '"></button>'
                if (aleftflag):
                    tareascargadas += '&nbsp;'
                    aleftflag = 0
                else:
                    tareascargadas += '<br><br>'
                    aleftflag = 1

        page = open("mobile/predefined.html").read()
        page = page.replace('<CARGARTAREAS>',tareascargadas)

       	print action

        if action != None:
        #  here the instruction to execute actionwhatever.mats
            tarea = yarp.Bottle()
            tarea.clear()
            if action == 'stop':
                tarea.addInt(-1)
                pt.write(tarea)
            else:
                tarea.addString(str(action))
                pt.write(tarea)

        return page
    predefined.exposed = True


    def video(self, camera=0):
    	print camera
        page = open("mobile/video.html").read()
        return page
    video.exposed = True


import os.path
current_dir = os.path.dirname(os.path.abspath(__file__))
interfaceconf = os.path.join(os.path.dirname(__file__), 'interface.conf')

if __name__ == '__main__':
    # CherryPy always starts with app.root when trying to map request URIs
    # to objects, so we need to mount a request handler root. A request
    # to '/' will be mapped to webAsibot().index().
    print current_dir
    yarp.Network.init()
    p = yarp.Port()
    p.open("/asibot_web"); # j commands
    px = yarp.Port()
    px.open("/asibot_x"); # x commands
    yarp.Network.connect("/asibot_x","/kdl_x")
    pt = yarp.Port()
    pt.open("/asibot_tasks"); # file commands
    yarp.Network.connect("/asibot_tasks","/file_cmd")

    cherrypy.config.update({'tools.staticdir.root': current_dir})
    cherrypy.quickstart(webAsibot(), config=interfaceconf)
    p.close();
else:
    # This branch is for the test suite; you can ignore it.
    cherrypy.tree.mount(webAsibot(), config=interfaceconf)
