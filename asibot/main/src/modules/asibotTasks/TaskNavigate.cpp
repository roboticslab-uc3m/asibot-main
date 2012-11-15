// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TaskNavigate.h"

/************************************************************************/
TaskNavigate::TaskNavigate() { }

/************************************************************************/
bool TaskNavigate::open(const ConstString& serverPrefix) {
    ConstString clientStr(serverPrefix);
    clientStr += "/mobile/rpc:o";
    rpcClient.open(clientStr);
    ConstString serverStr(serverPrefix);
    serverStr += "/mobile/rpc:i";
    if (!rpcClient.addOutput(serverStr)) {
        fprintf(stderr,"[error] Failed to find and/or connect to a mobile device.\n");
        return false;
    }
    printf("[success] Opened a mobile device.\n");
    return true;
}

/************************************************************************/
bool TaskNavigate::close() {
//    valid = false;
    rpcClient.interrupt();
    rpcClient.close();
    return true;
}

/************************************************************************/
bool TaskNavigate::run(const ConstString a, const ConstString b, const ConstString c, const ConstString d) {
    printf("[success] TaskNavigate::run(): begin.\n");
    Bottle miOutput, miInput, contents;
    //
    miInput.clear();
    miOutput.clear();
    contents.clear();
    miOutput.addVocab(VOCAB_SET);
    miOutput.addVocab(VOCAB_POSS);
    contents.addDouble(0);
    contents.addDouble(1.1);
    contents.addDouble(0);
    miOutput.addList() = contents;
    printf("Send %s...\n",miOutput.toString().c_str());
    rpcClient.write(miOutput, miInput);
    printf("Got %s.\n",miInput.toString().c_str());
    //
    miInput.clear();
    miOutput.clear();
    contents.clear();
    miOutput.addVocab(VOCAB_SET);
    miOutput.addVocab(VOCAB_POSS);
    contents.addDouble(1.3);
    contents.addDouble(0.6);
    contents.addDouble(1.57);
    miOutput.addList() = contents;
    printf("Send %s...\n",miOutput.toString().c_str());
    rpcClient.write(miOutput, miInput);
    printf("Got %s.\n",miInput.toString().c_str());
    //
    miInput.clear();
    miOutput.clear();
    contents.clear();
    miOutput.addVocab(VOCAB_SET);
    miOutput.addVocab(VOCAB_POSS);
    contents.addDouble(2.2);
    contents.addDouble(-0.1);
    contents.addDouble(0);
    miOutput.addList() = contents;
    printf("Send %s...\n",miOutput.toString().c_str());
    rpcClient.write(miOutput, miInput);
    printf("Got %s.\n",miInput.toString().c_str());
    //
    printf("[success] TaskNavigate::run(): end.\n");
    return true;
}

/************************************************************************/

