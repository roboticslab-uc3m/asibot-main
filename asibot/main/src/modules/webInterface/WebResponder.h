// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <yarp/os/all.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <fstream>
#include <sstream>

using namespace yarp::os;
using namespace yarp::dev;

using std::string;

/**
 *
 * @ingroup webResponder
 *
 * The WebResponder class manages YARP web RPCs as callbacks and additionally
 * manages resouces and parses HTML code before acting as a mini-server. Used by
 * WebInterface.
 *
 */
class WebResponder : public PortReader {
protected:
    bool simConnected, realConnected;
    ConstString htmlPath;
    ConstString resourcePath;
    string readFile(const ConstString& fileName);
    string& replaceAll(string& context, const string& from, const string& to);
    int stringToInt(const ConstString& inString);
    double stringToDouble(const ConstString& inString);

    yarp::dev::PolyDriver simDevice;
    yarp::dev::IPositionControl *simPos;

    yarp::dev::PolyDriver realDevice;
    yarp::dev::IPositionControl *realPos;

//    yarp::dev::PolyDriver cartesianDevice;
//    yarp::dev::ICartesianControl *icart;

public:
    ConstString getCss();
    bool init();
    bool closeDevices();
    bool read(ConnectionReader& in);
    bool setHtmlPath(const ConstString& _htmlPath);
    bool setResourcePath(const ConstString& _resourcePath);
};

#endif

