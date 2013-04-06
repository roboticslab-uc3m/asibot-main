// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ColorClient.hpp"

/************************************************************************/
ColorClient::ColorClient() { }

/************************************************************************/
bool ColorClient::open(const char* serverPrefix, bool quiet) {
    isQuiet = quiet;
    ConstString clientStr(serverPrefix);
    clientStr += "/state:i";
    stateClient.open(clientStr);
    ConstString serverStr(serverPrefix);
    serverStr += "/state:o";
    if (!Network::connect(serverStr,clientStr)) {
        if (!isQuiet) fprintf(stderr,"[fail] Could not connect to colorXxx, requires a running instance.\n");
        return false;
    }
    if (!isQuiet) printf("[ColorClient] Opened connection with colorXxx.\n");
    return true;
}

/************************************************************************/
bool ColorClient::close() {
    if (!isQuiet) printf("[ColorClient] Close...\n");
    stateClient.interrupt();
    stateClient.close();
    return true;
}

/************************************************************************/
bool ColorClient::get(const int &index, std::vector<double> &values) {
    if (!isQuiet) printf("[ColorClient] get(%d)\n",index);
    Bottle *miInput = stateClient.read();  // read(false) not reading...
    if(!miInput) {
        if (!isQuiet) fprintf(stderr,"[warning] Get failed, cannot read published state.\n");
        return false;
    }
    if (!isQuiet) printf("[ColorClient] Got %d elems: %s\n",miInput->size(),miInput->toString().c_str());
    if (index >= miInput->size()) {
        if (!isQuiet) fprintf(stderr,"[warning] Get failed. Index out of range.\n");
        return false;
    }
    Bottle *contents = miInput->get(index).asList();
    values.resize(contents->size());  // more efficient than push_back for large blocks
    for (int i=0; i<contents->size(); i++)
        values[i] = contents->get(i).asDouble();
    return true;
}

/************************************************************************/
bool ColorClient::size(int &value) {
    if (!isQuiet) printf("[ColorClient] size()\n");
    Bottle *miInput = stateClient.read();  // read(false) not reading...
    if(!miInput) {
        if (!isQuiet) fprintf(stderr,"[warning] Get failed, cannot read published state.\n");
        return false;
    }
    value = miInput->size();
    if (!isQuiet) printf("[ColorClient] Got: %d\n",value);
    return true;
}

/************************************************************************/


