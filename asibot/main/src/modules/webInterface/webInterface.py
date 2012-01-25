#! /usr/bin/env python

##
#
# @defgroup webInterface
#
# Serves a web interface using CherryPy to instanciate webAsibot.
# 
# \image html webInterface.png 
# <center> <a href="http://youtu.be/9B0tVJsg-0I" target="_blank"> Watch video on Youtube</a> </center>
#
# <b> Legal </b>
#
# Copyright: 2011 (C) Universidad Carlos III de Madrid
# 
# Authors: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G Victores</a>;
# Santiago Morante
#
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
#
# Please support this development by referencing it in your works/publications/projects with:
#
# \include victores2011interaccion.bib
# 
# <b>Installation</b>
#
# The module is installed when enable_ASIBOT_MODULES is activated (default). For further
# installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
#
# <b>Running</b> (assuming correct installation)
#
# yarp server &
#
# $ASIBOT_DIR/bin/webInterface.py
#
# <b>Modify</b>
# 
# This file can be edited at 
# $ASIBOT_ROOT/main/src/modules/webInterface/webInterface.py
#

# Import CherryPy global namespace (server)
import cherrypy

# Get some YARP networking
import yarp

# For nice subprocess
#from subprocess import call
import subprocess

# To fix the CRLF problems
import re

##
#
# The webAsibot class implements the web queries to the <a href="group__webInterface.html">webInterface</a>
# as function calls.
class webAsibot:
################################# INTERNAL PARAMETERS #########################
    asibot_state = 0
    sim_state = 0
    teachfile = "None"
    jointvals = [0.0, 0.0, 0.0, 0.0, 0.0]
    enc_state = 0
    executeid = "None"
    executefile = "None"

##################################### INDEX PAGE ##############################
    def index(self):
        page = open(os.path.join(shared_dir,page_format,'index.html')).read()
        return page
    index.exposed = True

##################################### JOINT PAGE ##############################
    def joint(self, joint=-1, movement=None, one=0, two=0, three=0, four=0, five=0, stop=0):
        strchain = "joint" + str(joint) + ", movement " + str(movement) + ", one " + str(one)
        strchain += ", two " + str(two) + ", three " + str(three) + ", four" + str(four) + ", five "
        strchain += str(five) + ", stop "+ str(stop)
        print strchain
  
        if int(joint) != -1:
            if movement == "left":
                if (self.sim_state == 1): posSim.relativeMove(int(joint)-1,-5.0)
                if (self.asibot_state == 1): posReal.relativeMove(int(joint)-1,-5.0)
            elif movement == "right":
                if (self.sim_state == 1): posSim.relativeMove(int(joint)-1,5.0)
                if (self.asibot_state == 1): posReal.relativeMove(int(joint)-1,5.0)

        elif movement == "absolute":
            if one != '':
                if (self.sim_state == 1): posSim.positionMove(0,float(one))
                if (self.asibot_state == 1): posReal.positionMove(0,float(one))
            if two != '':
                if (self.sim_state == 1): posSim.positionMove(1,float(two))
                if (self.asibot_state == 1): posReal.positionMove(1,float(two))
            if three != '':
                if (self.sim_state == 1): posSim.positionMove(2,float(three))
                if (self.asibot_state == 1): posReal.positionMove(2,float(three))
            if four != '':
                if (self.sim_state == 1): posSim.positionMove(3,float(four))
                if (self.asibot_state == 1): posReal.positionMove(3,float(four))
            if five != '':
                if (self.sim_state == 1): posSim.positionMove(4,float(five))
                if (self.asibot_state == 1): posReal.positionMove(4,float(five))
                
        elif movement == "relative":
            if one != '':
                if (self.sim_state == 1): posSim.relativeMove(0,float(one))
                if (self.asibot_state == 1): posReal.relativeMove(0,float(one))
            if two != '':
                if (self.sim_state == 1): posSim.relativeMove(1,float(two))
                if (self.asibot_state == 1): posReal.relativeMove(1,float(two))
            if three != '':
                if (self.sim_state == 1): posSim.relativeMove(2,float(three))
                if (self.asibot_state == 1): posReal.relativeMove(2,float(three))
            if four != '':
                if (self.sim_state == 1): posSim.relativeMove(3,float(four))
                if (self.asibot_state == 1): posReal.relativeMove(3,float(four))
            if five != '':
                if (self.sim_state == 1): posSim.relativeMove(4,float(five))
                if (self.asibot_state == 1): posReal.relativeMove(4,float(five))

        elif int(stop) == 1:
                if (self.sim_state == 1): posSim.stop()
                if (self.asibot_state == 1): posReal.stop()

        page = open(os.path.join(shared_dir,page_format,'joint.html')).read()
        return page
    joint.exposed = True    


##################################### CARTESIAN PAGE ##########################
    def cartesian(self, stop=0):
    	print stop
    	
    	if int(stop) == 1:
            botella=yarp.Bottle()
            botella.clear()
            botella.addInt(-1)
            if (self.sim_state == 1): pxSim.write(botella)
            if (self.asibot_state == 1): pxReal.write(botella)

        page = open(os.path.join(shared_dir,page_format,'cartesian.html')).read()
        return page
    cartesian.exposed = True


    def joypad(self, axis=None, origin=None, movement=None): # of cartesian tab
    	print axis
    	print origin
    	print movement
    	
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

            # already have good conditions
            if (self.sim_state == 1): pxSim.write(botella)
            if (self.asibot_state == 1): pxReal.write(botella)

        page = open(os.path.join(shared_dir,page_format,'cartesian.html')).read()
        return page
    joypad.exposed = True

    def numeric(self, origin=None, x=0, y=0, z=0, roll=0, pitch=0, yaw=0):
    	print origin
    	print x
    	print y
    	print z
    	print roll
    	print pitch
    	print yaw
    	
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

            # if origin != None
            if (self.sim_state == 1): pxSim.write(botella)
            if (self.asibot_state == 1): pxReal.write(botella)

        page = open(os.path.join(shared_dir,page_format,'cartesian.html')).read()
        return page
    numeric.exposed = True


##################################### SPEECH PAGE ############################

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

        page = open(os.path.join(shared_dir,page_format,'speech.html')).read()
        return page
    speech.exposed = True


##################################### TEACH PAGE ############################
    def teach(self, tfile=None):
        if tfile != None:
            self.teachfile = tfile
        return self.formatTeachPage()
    teach.exposed = True

    def create(self, nfile=None):
        if nfile != None:
            newfile = programs_dir + "/" + nfile + ".py"
            if os.path.isfile(newfile):
                print "already exists"
            else:
                NFILE = open(newfile,"w")
                templatefile = os.path.join(programs_dir,page_format,"template.py")
                if os.path.isfile(newfile):
                    print "apply template"
                    mystr = open(templatefile).read()
                    NFILE.write(mystr)
                NFILE.close()
                os.chmod(newfile,0755)
                print "created " + nfile
        return self.formatTeachPage()
    create.exposed = True

    def delete(self, dfile=None):
        if dfile != None and dfile != '':
            newfile = programs_dir + "/" + dfile + ".py"
            if os.path.isfile(newfile):
                print dfile + " exists"
                if os.remove(newfile):
                     print "success"
                if not os.path.isfile(newfile):
                     print dfile + " deleted"
        return self.formatTeachPage()
    delete.exposed = True


    def update(self, lstr=None):
        print 'entered save routine'
        print "\n\n\n"
        usedfile = programs_dir + "/" + self.teachfile + ".py"
        if not os.path.isfile(usedfile):
            print usedfile + " does not exist"
        else:
            SFILE = open(usedfile,"w")
            # CR = carriage return = \r 
            # LF = line feed = newline = \n 
            # linux uses LF = \n
            # Windows uses CR LF = \r\n 
            newdata = re.sub("\r\n", "\n", lstr)
    
            print 'entered update routine'
            if (self.asibot_state == 1): 
                updatedEncs = yarp.DVector(5)
                encReal.getEncoders(updatedEncs)
                self.jointvals[0] = float(updatedEncs[0])
                self.jointvals[1] = float(updatedEncs[1])
                self.jointvals[2] = float(updatedEncs[2])
                self.jointvals[3] = float(updatedEncs[3])
                self.jointvals[4] = float(updatedEncs[4])
                self.enc_state = 1
                newdata += "\nPx = robtarget("
                for i in range(len(self.jointvals)):
                    newdata += str(self.jointvals[i])
                    if not i == len(self.jointvals)-1:
                        newdata += ","
                newdata += ")\n"
            elif (self.sim_state == 1): 
                updatedEncs = yarp.DVector(5)
                encSim.getEncoders(updatedEncs)
                self.jointvals[0] = float(updatedEncs[0])
                self.jointvals[1] = float(updatedEncs[1])
                self.jointvals[2] = float(updatedEncs[2])
                self.jointvals[3] = float(updatedEncs[3])
                self.jointvals[4] = float(updatedEncs[4])
                self.enc_state = 2
                newdata += "\nPx = robtarget("
                for i in range(len(self.jointvals)):
                    newdata += str(self.jointvals[i])
                    if not i == len(self.jointvals)-1:
                       newdata += ","
                newdata += ")\n"
        SFILE.write(newdata)
        SFILE.close()
        print "saved to " + usedfile
        return self.formatTeachPage()
    update.exposed = True

    def save(self, lstr=None):
        print 'entered save routine\n'
        usedfile = programs_dir + "/" + self.teachfile + ".py"
        if not os.path.isfile(usedfile):
            print usedfile + " does not exist"
        else:
            SFILE = open(usedfile,"w")
            # CR = carriage return = \r 
            # LF = line feed = newline = \n 
            # linux uses LF = \n
            # Windows uses CR LF = \r\n 
            newdata = re.sub("\r\n", "\n", lstr)
            SFILE.write(newdata)
            SFILE.close()
            print "saved to " + usedfile
        return self.formatTeachPage()
    save.exposed = True


    def formatTeachPage(self):
        page = open(os.path.join(shared_dir,page_format,'teach.html')).read()
        # explorer implementation 
        ficheroscargados = ""
        ficheroscargadosD = ""
        matsfiles = os.listdir(programs_dir)
        for infile in matsfiles:
            if len(infile.split('.')) == 2:
                if (infile.split('.')[1] == "py"):
                    ficheroscargados += '<option value="' + infile.split('.')[0] + '">' + infile.split('.')[0] + '</option>'
                    if (infile.split('.')[0] != "home"):
                        ficheroscargadosD += '<option value="' + infile.split('.')[0] + '">' + infile.split('.')[0] + '</option>'

        page = page.replace('<CARGARFICHEROS>',ficheroscargados)
        page = page.replace('<CARGARFICHEROSD>',ficheroscargadosD)

        usedfile = programs_dir + "/" + self.teachfile + ".py"
        centraldefault = '<img src="images/teach.png" width="293" height="293">'
        centraldefault += '<script type="text/javascript">theFileInUse = ""</script>'
        if not os.path.isfile(usedfile):
            print usedfile + " does not exist"
            self.teachfile = ""
            page = page.replace('<CENTRALPIECE>',centraldefault)
        else:
            contenido = '<script type="text/javascript">theFileInUse = "' + self.teachfile + '"</script>'
            contenido += '<textarea name="lstr" style="width: 295px;height: 322px;">'
            contenido += open(usedfile).read()
            contenido += '</textarea>'
            page = page.replace('<CENTRALPIECE>',contenido)
        page = page.replace('<FNAME>',self.teachfile)

        if (self.asibot_state == 0) and (self.sim_state == 0):
            page = page.replace('<JOINTVALS>', '<script type="text/javascript">encState=-2</script>')
        else:
            if self.enc_state == 0:
                page = page.replace('<JOINTVALS>', '<script type="text/javascript">encState=-1</script>')
            else:
                mystr = '<script type="text/javascript">encState=0</script>'
                page = page.replace('<JOINTVALS>', mystr)
        return page


################################ PREDEFINED PAGE ##############################
    def predefined(self):
        tareascargadas = ''
        matsfiles = os.listdir(programs_dir)
        aleftflag = 1 # determines if on left or right side of the table
        for infile in matsfiles:
            if len(infile.split('.')) == 2:
                if (infile.split('.')[1] == "py") and (infile.split('.')[0] != "home"):
                    tareascargadas += '<button type="submit" name="afile" value="' + infile.split('.')[0] + '"><img src="images/' + infile.split('.')[0] + '.jpg" width="330" height="230" alt="Please upload jpg for ' + infile.split('.')[0] + '"></button>'
                    if (aleftflag):
                        tareascargadas += '&nbsp;'
                        aleftflag = 0
                    else:
                        tareascargadas += '<br><br>'
                        aleftflag = 1

        page = open(os.path.join(shared_dir,page_format,'predefined.html')).read()

        page = page.replace('<CARGARTAREAS>',tareascargadas)

        return page
    predefined.exposed = True


################################### EXECUTE PAGE ##############################
    def execute(self, afile=None, command=None):
        print afile
        print command
        if afile != None:
            self.executefile = afile
            fullpath = programs_dir + "/" + afile + ".py"
            print "going to execute: " + str(fullpath) + "\n"
            self.executeid = subprocess.Popen(fullpath, 0, None, subprocess.PIPE, subprocess.PIPE, None)
            page = open(os.path.join(shared_dir,page_format,'execute.html')).read()
            page = page.replace('<INEXECUTION>',self.executefile)
            yield page
            #call(self.executefile)
            #for l in self.executeid.stdout.readlines():
            #    print l
            #    yield l
            #data = self.executeid.stdout.readline()
            data = "Begin"
            self.mylog = ""
            while len(data) != 0:
                data = self.executeid.stdout.readline()
                print "Counted", len(data), "readline:"
                if len(data) != 0:
                    print data
                    self.mylog += str(data)
                    yield str(data)
            yield "</textarea><div align='center'><font size='6'><b>Program reached end.</b></font></div></div></div></body></html>"
        elif command == "stop":
            pq = yarp.Port()
            pq.open("/webInterface/stop/rpc:o")
            b = yarp.Bottle()
            b.addVocab(yarp.Vocab.encode("quit"))
            if not yarp.Network.connect("/webInterface/stop/rpc:o","/userProgram/quit"):
                print "[warning] not connected to /userProgram/quit\n"
            response = yarp.Bottle()
            pq.write(b,response);
            print "Got response: " + str(response.toString())
            page = open(os.path.join(shared_dir,page_format,'execute.html')).read()
            page = page.replace('<INEXECUTION>',self.executefile)
            yield page
            yield self.mylog
            if response.get(0).asVocab() == yarp.Vocab.encode("ok"):
                yield "</textarea><div align='center'><font size='6'><b>Program stopped.</b></font></div></div></div></body></html>"
            else:
                yield "</textarea><div align='center'><font size='6'><b>Tried to stop program.</b></font></div></div></div></body></html>"
        elif command == "terminate":
            self.executeid.terminate()
            page = open(os.path.join(shared_dir,page_format,'execute.html')).read()
            page = page.replace('<INEXECUTION>',self.executefile)
            yield page
            yield self.mylog
            yield "</textarea><div align='center'><font size='6'><b>Program terminated.</b></font></div></div></div></body></html>"
        elif command == "kill":
            self.executeid.kill()
            page = open(os.path.join(shared_dir,page_format,'execute.html')).read()
            page = page.replace('<INEXECUTION>',self.executefile)
            yield page
            yield self.mylog
            yield "</textarea><div align='center'><font size='6'><b>Program killed.</b></font></div></div></div></body></html>"

            #yield str(self.predefined())
            #    yield self.executeid.stdout.readline() 
            #    print self.executeid.stdout.readline() 
            #    print self.executeid.stdout.read()
            #    yield self.executeid.stdout.read()
            #    output_file.write(l)
            #    while self.executeid.stdout.readlines():
            #    print self.executeid.stdout.readlines()
            #    yield self.executeid.stdout
#        page = open(os.path.join(shared_dir,page_format,'execute.html')).read()
#        page = page.replace('<INEXECUTION>',afile)
#        return page
    execute._cp_config = {'response.stream': True}
    execute.exposed = True

#################################### VIDEO PAGE ###############################
    def video(self, camera=0):
    	print camera
        portint = yarp.Network.queryName("/ravebot/img:o").getPort()
        print portint
        completeroute = "http://localhost:" + str(portint) + "/?action"
        page = open(os.path.join(shared_dir,page_format,'video.html')).read()
        page = page.replace('<SIMCAMIP>',completeroute)
        return page
    video.exposed = True

#    def test(self):
#        portint = yarp.Network.queryName("/ravebot/img:o").getPort()
#        print portint
#        completeroute = "localhost:" + str(portint) + "/?action"
#        page = open(os.path.join(shared_dir,page_format,'video.html')).read()
#        return page
#    test.exposed = True

################################### ADVANCED PAGE ##############################
    def advanced(self, action=None):
    	print action
        page = open(os.path.join(shared_dir,page_format,'advanced.html')).read()
        return page
    advanced.exposed = True


##################################### MODE PAGE ###############################
    def mode(self, option=None):
    	print option
        page = open(os.path.join(shared_dir,page_format,'mode.html')).read()

        if option == "asibot_con":
            self.asibot_state = 1
        if option == "asibot_dis":
            self.asibot_state = 0

        if option == "sim_con":
            self.sim_state = 1
        if option == "sim_dis":
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


##################################### PRE-MAIN ################################
import os.path
shared_dir = os.path.join(os.environ['ASIBOT_DIR'],'app','webInterface')
programs_dir = os.path.join(os.environ['ASIBOT_DIR'],'app','userPrograms')
interface_conf = os.path.join(shared_dir,'interface.conf')
page_format = 'asibot'

#current_dir = os.path.dirname(os.path.abspath(__file__))
#interfaceconf = os.path.join(os.path.dirname(__file__), 'interface.conf')

##################################### MAIN ####################################
if __name__ == '__main__':
    # CherryPy always starts with app.root when trying to map request URIs
    # to objects, so we need to mount a request handler root. A request
    # to '/' will be mapped to webAsibot().index().
    #print current_dir
    yarp.Network.init()
    if yarp.Network.checkNetwork() != True:
        print "[error] Please try running yarp server\n"
        quit()

    # Try connecting to real robot (remote)
    optionsReal = yarp.Property()
    optionsReal.put('local','/webInterface/canbot')
    optionsReal.put('remote','/canbot')
    optionsReal.put('device','remote_controlboard')
    ddReal = yarp.PolyDriver()
    if not ddReal.open(optionsReal):
        print "[warning] canbot device not available"
        print "[warning] Please try running test_canbot\n"
    else:
        print "[success] success connecting to canbot device\n"
    posReal = ddReal.viewIPositionControl()
    velReal = ddReal.viewIVelocityControl()
    encReal = ddReal.viewIEncoders()

    # Open port for cartesian moves of (remote) real robot
    pxReal = yarp.Port()
    pxReal.open("/webInterface/canbot/x/rpc:o")
    if not yarp.Network.connect("/webInterface/canbot/x/rpc:o","/canbot/x/rpc:i"):
        print "[warning] not connected to /canbot/x/rpc:i\n"
    else:
        print "[success] success connecting to /canbot/x/rpc:i\n"

    # Try connecting to simulated robot (remote)
    optionsSim = yarp.Property()
    optionsSim.put('local','/webInterface/ravebot')
    optionsSim.put('remote','/ravebot')
    optionsSim.put('device','remote_controlboard')
    ddSim = yarp.PolyDriver()
    if not ddSim.open(optionsSim):
        print "[warning] ravebot device not available"
        print "[warning] Please try running $ASIBOT_DIR/bin/test_kdl\n"
    else:
        print "[success] success connecting to ravebot device\n"
    posSim = ddSim.viewIPositionControl()
    velSim = ddSim.viewIVelocityControl()
    encSim = ddSim.viewIEncoders()

    # Open port for cartesian moves of (remote) simulated robot
    pxSim = yarp.Port()
    pxSim.open("/webInterface/ravebot/x/rpc:o")
    if not yarp.Network.connect("/webInterface/ravebot/x/rpc:o","/ravebot/x/rpc:i"):
        print "[warning] not connected to /ravebot/x/rpc:i\n"
    else:
        print "[success] success connecting to /ravebot/x/rpc:i\n"

    #cherrypy.config.update({'tools.staticdir.root': current_dir})
    cherrypy.config.update({'tools.staticdir.root': shared_dir})
    cherrypy.quickstart(webAsibot(), config=interface_conf)

    # On ctrl-c close all ports
    print "begin close ports"
    ddSim.close()
    ddReal.close()
    print "end close ports"

else:
    # This branch is for the test suite; you can ignore it.
    cherrypy.tree.mount(webAsibot(), config=interfaceconf)

