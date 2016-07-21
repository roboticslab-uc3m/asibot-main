// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <yarp/os/all.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "CartesianClient.h"

#include <fstream>
#include <sstream>
#include <dirent.h>  // for listing directory contents

#include <stdlib.h>  // for system

#define JOYPAD_RELMOVE 5  // [deg]
#define CJOYPAD_RELMOVE_POS 0.10  // [m]
#define CJOYPAD_RELMOVE_ORI 15  // [deg]
#define NUM_CART_COORDS 5  // 

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
    ResourceFinder rf;
    bool simConnected, realConnected;
    ConstString userPath;
    ConstString resourcePath;
    string readFile(const ConstString& filePath);  // needs absoulte path
    string readHtml(const ConstString& fileName);  // grabs from htmlPath
    bool appendToFile(const ConstString& absFile, const ConstString& inString); // writes to userPath
    bool rewriteFile(const ConstString& absFile, const ConstString& inString); // writes to userPath
    bool deleteFile(const ConstString& absFile); // needs absoulte path
    string& replaceAll(string& context, const string& from, const string& to);
    int stringToInt(const ConstString& inString);
    double stringToDouble(const ConstString& inString);
    ConstString doubleToString(const double& inDouble);
    ConstString intToString(const int& inInt);
    ConstString pointButtonCreator(const ConstString& pointsFile);
    ConstString wordOptionCreator(const ConstString& wordsFile);
    ConstString fileListCreator();
    ConstString taskListCreator();
    ConstString taskButtonCreator();

    yarp::dev::PolyDriver simDevice;
    yarp::dev::IPositionControl *simPos;
    CartesianClient *simCart;  // != ICartesianControl

    yarp::dev::PolyDriver realDevice;
    yarp::dev::IPositionControl *realPos;
    CartesianClient *realCart;  // != ICartesianControl

    double captureX[NUM_CART_COORDS];
    ConstString lastEditName;

public:
    ConstString getCss();
    bool init();
    bool closeDevices();
    bool read(ConnectionReader& in);
    bool setResourceFinder(ResourceFinder &rf);
    bool setUserPath(const ConstString& _userPath);
    bool setResourcePath(const ConstString& _resourcePath);
};

#endif

