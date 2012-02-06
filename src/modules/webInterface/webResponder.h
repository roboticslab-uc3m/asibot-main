// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WEB_RESPONDER__
#define __WEB_RESPONDER__

#include <yarp/os/all.h>

using namespace yarp::os;

class WebResponder : public PortReader {
public:
    ConstString getCss();
    bool read(ConnectionReader& in);
};

#endif

