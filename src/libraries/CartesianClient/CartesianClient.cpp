// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CartesianClient.h"

/************************************************************************/
CartesianClient::CartesianClient() { }

/************************************************************************/
bool CartesianClient::open() {
    rpcClient.open("/cartesianClient/rpc:o");
    if (!rpcClient.addOutput("/cartesianServer/rpc:i")) {
        printf("[error] no cartesian connection possible\n");
    }
    //valid = true;
    //else valid = false;
    return true;
}

/************************************************************************/
//bool CartesianClient::isValid() {
//    return valid;
//}

/************************************************************************/
bool CartesianClient::close() {
//    valid = false;
    rpcClient.interrupt();
    rpcClient.close();
    return true;
}

/************************************************************************/
bool CartesianClient::stat(double *x) {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_STAT);
    rpcClient.write(miOutput, miInput);
//    printf("Got %s\n",miInput.toString().c_str());
    Bottle* data = miInput.get(0).asList();
    for (int i=0; i<data->size(); i++)
        x[i] = data->get(i).asDouble();
    return true;
}

/************************************************************************/
bool CartesianClient::inv(const double *xd) {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_INV);
    Bottle dBottle;
    for(int i=0;i<NUM_AXES;i++)
        dBottle.addDouble(xd[i]);
    miOutput.addList() = dBottle;
    rpcClient.write(miOutput, miInput);
    return true;
}

/************************************************************************/
bool CartesianClient::movj(const double *xd) {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_MOVJ);
    Bottle dBottle;
    for(int i=0;i<NUM_AXES;i++)
        dBottle.addDouble(xd[i]);
    miOutput.addList() = dBottle;
    rpcClient.write(miOutput, miInput);
    return true;
}

/************************************************************************/
bool CartesianClient::movl(const double *xd) {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_MOVL);
    Bottle dBottle;
    for(int i=0;i<NUM_AXES;i++)
        dBottle.addDouble(xd[i]);
    miOutput.addList() = dBottle;
    rpcClient.write(miOutput, miInput);
    return true;
}

/************************************************************************/
bool CartesianClient::stop() {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_MY_STOP);
    rpcClient.write(miOutput, miInput);
    return true;
}

