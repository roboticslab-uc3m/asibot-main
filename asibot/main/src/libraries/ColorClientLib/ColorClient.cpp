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
        if (!isQuiet) fprintf(stderr,"[ColorClient] fail: Could not connect %s to %s.\n",serverStr.c_str(),clientStr.c_str());
        return false;
    }
    if (!isQuiet) printf("[ColorClient] success: %s reading from %s.\n",clientStr.c_str(),serverStr.c_str());
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
bool ColorClient::get(std::vector<double> &values) {
    values.clear();  // Wipe the contents.
    if (!isQuiet) printf("[ColorClient] get()\n");
    Bottle *miInput = stateClient.read();  // read(false) not reading...
    if(!miInput) {
        if (!isQuiet) fprintf(stderr,"[ColorClient] warning: Get failed, cannot read published state.\n");
        return false;
    }
    if (!isQuiet) printf("[ColorClient] Got %d elems: %s\n",miInput->size(),miInput->toString().c_str());
    if(miInput->size() == 0) {
        if (!isQuiet) fprintf(stderr,"[ColorClient] warning: Got empty contents, returning false.\n");
        return false;
    }
    for(int i=0;i<miInput->size();i++){
        Bottle *contents = miInput->get(i).asList();
        for (int j=0; j<contents->size(); j++)
            values.push_back(contents->get(j).asDouble());
    }
    return true;
}

/************************************************************************/
bool ColorClient::size(int &value) {
    if (!isQuiet) printf("[ColorClient] size()\n");
    Bottle *miInput = stateClient.read();  // read(false) not reading...
    if(!miInput) {
        if (!isQuiet) fprintf(stderr,"[ColorClient] warning: Get failed, cannot read published state.\n");
        return false;
    }
    if(miInput->size() == 0) {
        if (!isQuiet) fprintf(stderr,"[ColorClient] warning: Got empty contents, returning false and size -1.\n");
        value = -1;
        return false;
    }
    Bottle *contents = miInput->get(0).asList();
    value = contents->size();
    if (!isQuiet) printf("[ColorClient] Got: %d\n",value);
    return true;
}

/************************************************************************/


