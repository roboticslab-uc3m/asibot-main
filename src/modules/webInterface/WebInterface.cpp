// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WebInterface.h"

/************************************************************************/
WebInterface::WebInterface() { }

/************************************************************************/
bool WebInterface::configure(ResourceFinder &rf) {

    period = DEFAULT_PERIOD;  // double
    ConstString resources = DEFAULT_RESOURCES;
    ConstString webIp = DEFAULT_WEB_IP;
    int webPort = DEFAULT_WEB_PORT;
    
    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("WebInterface Options:\n");
        printf("\t--period [s] (default: \"%f\")\n",period);
        printf("\t--resources (default: \"%s\")\n",resources.c_str());
        printf("\t--webIp (default: \"%s\")\n",webIp.c_str());
        printf("\t--webPort (default: \"%d\")\n",webPort);
    }

    counter = 0;
    if(rf.check("period")) period = rf.find("period").asDouble();
    if(rf.check("resources")) resources = rf.find("resources").asString();
    if(rf.check("webIp")) webIp = rf.find("webIp").asString();
    if(rf.check("webPort")) webPort = rf.find("webPort").asInt();
    printf("WebInterface using period: %f, resources: %s.\n", period,resources.c_str());
    printf("WebInterface using webIp: %s, webPort: %d.\n",webIp.c_str(),webPort);

    responder.setResourceFinder(rf);
    ConstString userPath = rf.getHomeContextPath() + "/";
    printf("WebInterface using userPath: %s\n",userPath.c_str());
    responder.setUserPath(userPath);
    ConstString resourcePath = "http://";
    resourcePath += resources + "/";
    printf("WebInterface using resourcePath: %s\n",resourcePath.c_str());
    responder.setResourcePath(resourcePath);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    responder.init();
    server.setReader(responder);

    ConstString name = rf.check("name",Value("/web")).asString();

    contact = Contact::byName(name);
    if (webPort!=0) {
        contact = contact.addSocket("",webIp,webPort);
    }
    if (!server.open(contact)) return false;
    contact = server.where();

    return true;
}

/************************************************************************/
bool WebInterface::updateModule() {
    printf("Server running, visit: http://%s:%d/index\n",
                   contact.getHost().c_str(),
                   contact.getPort());
    Bottle push;
    push.addString("web");
    std::ostringstream s;
    s << counter;
    ConstString div = ConstString("<div>")+s.str()+" counter count</div>";
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
    responder.closeDevices();
    server.close();
    return true;
}

