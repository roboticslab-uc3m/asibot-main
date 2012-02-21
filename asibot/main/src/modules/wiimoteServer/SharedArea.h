
#ifndef __SHAREDAREA_H__
#define __SHAREDAREA_H__

#include <yarp/os/all.h>

using namespace yarp::os;

#define VOCAB_FWD VOCAB3('f','w','d')
#define VOCAB_BKWD VOCAB4('b','k','w','d')
#define VOCAB_MY_STOP VOCAB4('s','t','o','p')
#define VOCAB_NULL VOCAB4('n','u','l','l')

class SharedArea {
private:
    int cmd;
    double pitch, roll;
    Semaphore cmdMutex, pitchMutex, rollMutex;
public:
    void setCmd(const int& _cmd);
    void setPitch(const double& _pitch);
    void setRoll(const double& _roll);
    int getCmd();
    double getPitch();
    double getRoll();
};

#endif

