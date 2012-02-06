// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <fstream>
#include <yarp/os/all.h>

using namespace yarp::os;
using std::string;

class WebResponder : public PortReader {
protected:
    ResourceFinder rf;
    ConstString htmlPath;
    ConstString resourcePath;
    string readFile(const ConstString& fileName);
    string& replaceAll(string& context, const string& from, const string& to);
public:
    ConstString getCss();
    bool read(ConnectionReader& in);
    bool setHtmlPath(const ConstString& _htmlPath);
    bool setResourcePath(const ConstString& _resourcePath);
};

#endif

