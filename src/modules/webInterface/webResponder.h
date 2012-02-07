// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <yarp/os/all.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <fstream>

using namespace yarp::os;
using namespace yarp::dev;

using std::string;

/**
 *
 * @ingroup webResponder
 *
 * The WebResponder class manages YARP web RPCs as callbacks and additionally
 * manages resouces and parses HTML code before acting as a mini-server.
 *
 */
class WebResponder : public PortReader {
protected:
    bool simConnected, realConnected;
    ResourceFinder rf;
    ConstString htmlPath;
    ConstString resourcePath;
    string readFile(const ConstString& fileName);
    string& replaceAll(string& context, const string& from, const string& to);

    yarp::dev::PolyDriver cartesianDevice;
    yarp::dev::PolyDriver realDevice;
    yarp::dev::PolyDriver simDevice;

    yarp::dev::ICartesianControl *icart;
    yarp::dev::IPositionControl *ipos;

public:
    ConstString getCss();
    bool init();
    bool closeDevices();
    bool read(ConnectionReader& in);
    bool setHtmlPath(const ConstString& _htmlPath);
    bool setResourcePath(const ConstString& _resourcePath);
};

#endif

