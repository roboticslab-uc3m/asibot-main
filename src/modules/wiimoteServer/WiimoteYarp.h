// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __WIIMOTE_YARP__
#define __WIIMOTE_YARP__

#include "yarp/os/all.h"
#include "SharedArea.h"

using namespace yarp::os;

#define DEFAULT_MSPORTOUT 300  // In ms

class WiimoteYarp : public RateThread {
protected:
	Network yarp;
    BufferedPort<Bottle> port;
    SharedArea* pSharedArea;
    double msPortOut; // [ms]
public:
    WiimoteYarp() : RateThread(DEFAULT_MSPORTOUT) { }  // In ms
    void setSharedArea(SharedArea* _pSharedArea);
    bool open();
    bool close();
    void run();  // The periodical function
    void forceStop();
};

#endif

