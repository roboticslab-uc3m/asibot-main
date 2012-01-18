// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_SERVER__
#define __CARTESIAN_SERVER__

#include <yarp/os/RFModule.h>
#include <yarp/os/Module.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#define DEFAULT_CONTROLLER "cartesianbot"

using namespace yarp::os;
using namespace yarp::dev;

/*class xCommPort : public BufferedPort<Bottle> {
protected:
    yarp::dev::ICartesianControl *icart;
    virtual void onRead(Bottle& b) {
        printf("[Debug] Data arrived on xCommPort\n");
        printf("Recieved %s.\n", b.toString().c_str());
        int choice = b.get(0).asInt();
        if (b.get(0).getCode() != BOTTLE_TAG_INT) choice = -2;
        if (choice==-1) {  ///////////////////////////////// x -1 /////////////////////////////////
            Bottle replyB;
            if(!icart->stopControl())
                replyB.addVocab(VOCAB_OK);
            else
                replyB.addVocab(VOCAB_FAILED);
            this.reply(replyB);
        } else if (choice==0) { ///////////////////////////////// x 0 /////////////////////////////////
//            bottle_x_o = theControlLoop.controlStat();
//            port_x.reply(bottle_x_o);
        }
    }
public:
    void setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
        icart = _icart;
    }

};*/

// PortProcessor class will help us create a callback&rpc port
class PortProcessor : public PortReader {
protected:

    yarp::dev::ICartesianControl *icart;

    virtual bool read(ConnectionReader& connection) {
        Bottle in, out;
        in.read(connection);
        printf("Got %s\n", in.toString().c_str());  
        out.clear();
        ConnectionWriter *returnToSender = connection.getWriter();
        int choice = in.get(0).asInt();
        if (in.get(0).getCode() != BOTTLE_TAG_INT) choice = -2;
        if (choice==-1) {  ///////////////////////////////// x -1 /////////////////////////////////
            if(icart->stopControl())
                out.addVocab(VOCAB_OK);
            else
                out.addVocab(VOCAB_FAILED);
            if (returnToSender!=NULL) {
                out.write(*returnToSender);
            }
            return true;
//        } else if (choice==0) { ///////////////////////////////// x 0 /////////////////////////////////
//            bottle_x_o = theControlLoop.controlStat();
//            port_x.reply(bottle_x_o);
        }
        out.addVocab(VOCAB_FAILED);
        if (returnToSender!=NULL) {
            out.write(*returnToSender);
        }
    }
public:
    void setCartesianInterface(yarp::dev::ICartesianControl* _icart) {
        icart = _icart;
    }

};

class cartesianServer : public RFModule {
protected:
    yarp::dev::PolyDriver cartesianDevice;

    yarp::dev::ICartesianControl *icart;

	PortProcessor xProcessor;
    Port xPort;
    
    bool updateModule();
    bool interruptModule();
//    double getPeriod();
//    int period;

public:
    cartesianServer();
    bool configure(ResourceFinder &rf);
};

#endif

