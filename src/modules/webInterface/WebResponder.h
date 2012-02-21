// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <yarp/os/all.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "CartesianClient.h"

#include <fstream>
#include <sstream>

#define JOYPAD_RELMOVE 5  // [deg]
#define CJOYPAD_RELMOVE_POS 0.10  // [m]
#define CJOYPAD_RELMOVE_ORI 15  // [deg]

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
    CartesianClient *simCart;  // != ICartesianControl

    yarp::dev::PolyDriver realDevice;
    yarp::dev::IPositionControl *realPos;
    CartesianClient *realCart;  // != ICartesianControl

public:
    ConstString getCss();
    bool init();
    bool closeDevices();
    bool read(ConnectionReader& in);
    bool setHtmlPath(const ConstString& _htmlPath);
    bool setResourcePath(const ConstString& _resourcePath);
};

#endif

