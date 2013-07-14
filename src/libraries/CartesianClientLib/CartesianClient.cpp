// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CartesianClient.h"

/************************************************************************/
CartesianClient::CartesianClient() { }

/************************************************************************/
bool CartesianClient::open(const ConstString& serverPrefix) {
    ConstString clientStr(serverPrefix);
    clientStr += "/cartesianServer/rpc:o";
    rpcClient.open(clientStr);
    ConstString serverStr(serverPrefix);
    serverStr += "/cartesianServer/rpc:i";
    if (!rpcClient.addOutput(serverStr)) {
        fprintf(stderr,"[CartesianClient] error: no cartesianServer connection possible\n");
        return false;
    }
    printf("[CartesianClient] success: Opened connection with cartesianServer.\n");
    return true;
}

/************************************************************************/
bool CartesianClient::close() {
//    valid = false;
    printf("CartesianClient close...\n");
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
    Bottle* data = miInput.get(1).asList();
    for (int i=0; i<data->size(); i++)
        x[i] = data->get(i).asDouble();
    return true;
}

/************************************************************************/
bool CartesianClient::inv(const double *xd, double *q) {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_INV);
    Bottle dBottle;
    for(int i=0;i<NUM_AXES;i++)
        dBottle.addDouble(xd[i]);
    miOutput.addList() = dBottle;
    rpcClient.write(miOutput, miInput);
    Bottle* data = miInput.get(0).asList();
    for (int i=0; i<data->size(); i++)
        q[i] = data->get(i).asDouble();
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
bool CartesianClient::wait() {
    Bottle miOutput, miInput;
    miOutput.clear();
    miOutput.addVocab(VOCAB_WAIT);
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

/************************************************************************/

