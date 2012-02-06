// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "webInterface.h"

/************************************************************************/
WebInterface::WebInterface() { }

/************************************************************************/
bool WebInterface::configure(ResourceFinder &rf) {

    counter = 0;
    period = rf.check("period",5,"period in s").asDouble();

    ConstString htmlPath = rf.getContextPath() + "/../html/";
    printf("htmlPath: %s\n",htmlPath.c_str());
    responder.setHtmlPath(htmlPath);
    ConstString resourcePath = "http://";
    resourcePath += rf.check("resource","localhost","resource path").asString() + "/";
    printf("resourcePath: %s\n",resourcePath.c_str());
    responder.setResourcePath(resourcePath);
    server.setReader(responder);

    ConstString name = rf.check("name",Value("/web")).asString();
    int port_number = rf.check("port",Value(0)).asInt();

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
    Bottle push;
    push.addString("web");
    ConstString div = ConstString("<div>")+ConstString::toString(counter)+" counter count</div>";
    push.addString(div);
    server.write(push);
    counter++;
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

