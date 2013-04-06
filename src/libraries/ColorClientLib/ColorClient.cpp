// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorClient.hpp"

/************************************************************************/
ColorClient::ColorClient() { }

/************************************************************************/
bool ColorClient::open(const char* serverPrefix, bool quiet) {
    isQuiet = quiet;
    ConstString clientStr(serverPrefix);
    clientStr += "/state:i";
    rpcClient.open(clientStr);
    ConstString serverStr(serverPrefix);
    serverStr += "/state:i";
    if (!rpcClient.addOutput(serverStr)) {
        if (!isQuiet) fprintf(stderr,"[fail] Could not connect to colorXxx, requires a running instance.\n");
        return false;
    }
    if (!isQuiet) printf("[ColorClient] Opened connection with colorXxx.\n");
    return true;
}

/************************************************************************/
bool ColorClient::close() {
    if (!isQuiet) printf("[ColorClient] Close...\n");
    rpcClient.interrupt();
    rpcClient.close();
    return true;
}

/************************************************************************/
bool ColorClient::get(const int &index, std::vector<double> &values) {
    if (!isQuiet) printf("[ColorClient] get(%d)\n",feature);
    Bottle miInput;
    colorClient.read(miInput);
/*    if ((miInput.get(0).getCode() == BOTTLE_TAG_VOCAB)&&(miInput.get(0).asVocab() == VOCAB_FAILED)) {
        if (!isQuiet) fprintf(stderr,"[warning] Get failed. Possibly non-existent word or feature out of range.\n");
        return false;
    }
    if (!isQuiet) printf("[ColorClient] Got %d elems: %s\n",miInput.size(),miInput.toString().c_str());
    values.resize(miInput.size());  // more efficient than push_back for large blocks
    for (int i=0; i<miInput.size(); i++)
        values[i] = miInput.get(i).asDouble();*/
    return true;
}

/************************************************************************/
bool ColorClient::size(const char *word, int &value) {
    if (!isQuiet) printf("[ColorClient] size(%s)\n",word);
    Bottle miOutput, miInput;
    miOutput.addString("size");
    miOutput.addString(word);
    rpcClient.write(miOutput, miInput);
    if ((miInput.get(0).getCode() == BOTTLE_TAG_VOCAB)&&(miInput.get(0).asVocab() == VOCAB_FAILED)) {
        if (!isQuiet) fprintf(stderr,"[warning] Get failed. Possibly non-existent word or feature out of range.\n");
        return false;
    }
    value = miInput.get(0).asInt();
    if (!isQuiet) printf("[ColorClient] Got: %d\n",value);
    return true;
}

/************************************************************************/


