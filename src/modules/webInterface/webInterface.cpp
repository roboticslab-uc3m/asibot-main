// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "webInterface.h"

/************************************************************************/
WebInterface::WebInterface() { }

/************************************************************************/
bool WebInterface::configure(ResourceFinder &rf) {

    period = rf.check("period",5,"period in s").asInt();

    server.setReader(responder);

    ConstString name = rf.check("name",Value("/web")).asString();
    int port_number = rf.check("p",Value(0)).asInt();

    contact = Contact::byName(name);
    if (port_number!=0) {
        contact = contact.addSocket("","",port_number);
    }
    if (!server.open(contact)) return false;
    contact = server.where();

    printf("[success] WebInterface configured.\n");
    return true;
}

/************************************************************************/
bool WebInterface::updateModule() {
    printf("Server running, visit: http://%s:%d/test\n",
                   contact.getHost().c_str(),
                   contact.getPort());
    return true;
}

/************************************************************************/
double WebInterface::getPeriod() {
    return period;  // seconds
}

/************************************************************************/
bool WebInterface::interruptModule() {
    server.interrupt();
    server.close();
    return true;
}

