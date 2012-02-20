// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __CARTESIAN_CLIENT__
#define __CARTESIAN_CLIENT__

#include <yarp/os/all.h>

#define NUM_AXES 5
#define VOCAB_MY_STOP VOCAB4('s','t','o','p')
#define VOCAB_STAT VOCAB4('s','t','a','t')
#define VOCAB_MOVL VOCAB4('m','o','v','l')
#define VOCAB_MOVJ VOCAB4('m','o','v','j')
#define VOCAB_INV VOCAB3('i','n','v')

using namespace yarp::os;

class CartesianClient {
protected:
    RpcClient rpcClient;
//    bool valid;
public:
    CartesianClient();
    bool open();
//    bool isValid();
    bool close();
    bool stat(double *xd);
    bool inv(const double *xd);
    bool movj(const double *xd);
    bool movl(const double *xd);
    bool stop();
};

#endif

