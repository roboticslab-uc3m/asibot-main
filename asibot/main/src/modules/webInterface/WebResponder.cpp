// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "WebResponder.h"

/************************************************************************/
bool WebResponder::init() {
    simConnected = false;
    realConnected = false;
    simPos = 0;
    realPos = 0;
    simCart = 0;
    realCart = 0;
    return true;
}

/************************************************************************/
bool WebResponder::closeDevices() {
    if(realDevice.isValid())
        realDevice.close();
    if(simDevice.isValid())
        simDevice.close();
    if(simCart) {
        simCart->close();
        delete simCart;
        simCart = 0;
    }
    if(realCart) {
        simCart->close();
        delete simCart;
        simCart = 0;
    }
    return true;
}

/************************************************************************/
bool WebResponder::setHtmlPath(const ConstString& _htmlPath) {
    htmlPath = _htmlPath;
    return true;
}

/************************************************************************/
bool WebResponder::setResourcePath(const ConstString& _resourcePath) {
    resourcePath = _resourcePath;
    return true;
}

/************************************************************************/
string& WebResponder::replaceAll(string& context, const string& from, const string& to) {
    // thank you Bruce Eckel for this one!! (TICPP-2nd-ed-Vol-two)
    size_t lookHere = 0;
    size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != string::npos) {
        context.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
    }
    return context;
}

/************************************************************************/
string WebResponder::readFile(const ConstString& fileName) {
    ConstString filePath = htmlPath + fileName;
    printf("filePath: %s\n",filePath.c_str());
    // thank you Tyler McHenry @ nerdland.net and KeithB @ ndssl.vbi.vt.edu for this algorithm
    // link: http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring [2012-02-06]
    std::ifstream t(filePath.c_str());
    std::string str;
    t.seekg(0, std::ios::end);   
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    t.close();
    ConstString resourceURL = resourcePath + "fig/";
    replaceAll(str, "fig/", resourceURL.c_str());
    if(simConnected) replaceAll(str, "simInit.jpg", "simCon.jpg");
    else replaceAll(str, "simInit.jpg", "simDis.jpg");
    if(realConnected) replaceAll(str, "realInit.jpg", "realCon.jpg");
    else replaceAll(str, "realInit.jpg", "realDis.jpg");
    return str;
}

/************************************************************************/
int WebResponder::stringToInt(const ConstString& inString) {
    int outInt;
    std::istringstream buffer(inString.c_str());
    buffer >> outInt;
    if(inString=="") outInt = 0;
    return outInt;
}

/************************************************************************/
double WebResponder::stringToDouble(const ConstString& inString) {
    double outDouble;
    std::istringstream buffer(inString.c_str());
    buffer >> outDouble;
    if(inString=="") outDouble = 0.0;
    return outDouble;
}

/************************************************************************/
ConstString WebResponder::getCss() {
    return ConstString(readFile("style.css").c_str());
}

/************************************************************************/
bool WebResponder::read(ConnectionReader& in) {
    Bottle request, response;
    if (!request.read(in)) return false;
    printf("Request: %s\n", request.toString().c_str());
    ConnectionWriter *out = in.getWriter();
    if (out==NULL) return true;
    response.addString("web");

    ConstString code = request.get(0).asString();
    if (code=="style.css") {
        response.addString(getCss());
        response.addString("mime");
        response.addString("text/css");
        return response.write(*out);
    } else if (code=="xmlhttp.js") {
        response.addString(readFile("xmlhttp.js").c_str());
        return response.write(*out);
    } else if (code=="connectionTab.js") {
        response.addString(readFile("connectionTab.js").c_str());
        return response.write(*out);
    } else if (code=="testEqual") {
        response.addString(readFile("testEqual.html").c_str());
        return response.write(*out);
    } else if (code=="equal.1") {
        ConstString inParam = request.find("a").asString();
        printf("Got an %s, going to equal it.\n",inParam.c_str());
        response.addString(inParam);
        return response.write(*out);
    } else if (code=="connectReal.1") {
        ConstString inParam = request.find("real").asString();
        printf("Got %s. ",inParam.c_str());
        ConstString outParam;
        if (realConnected){
            printf("Disconnecting from real robot.\n");
            realDevice.close();
            realConnected = false;
            realPos = 0;
            // Maybe perform some checks here
            outParam = "REALOFF";
        } else {
            printf("Connecting to real robot.\n");
            Property options;
            options.put("device","remote_controlboard");
            options.put("remote","/canbot");
            options.put("local","/webLocal");
            realDevice.open(options);
            bool ok = true;        
            if(!realDevice.isValid()) {
                printf("[error] canbot device not available.\n");
                ok = false;
            } else printf ("[success] canbot device available.\n");
            if(!realDevice.view(realPos)) {
                printf("[error] canbot interface not available.\n");
                ok = false;
            } else printf ("[success] canbot interface available.\n");
            if (!ok) {
                realDevice.close();
                realConnected = false;
                realPos = 0;
                outParam = "REALOFF";
            } else {
                realConnected = true;
                outParam = "REALON";
            }
        }
        response.addString(outParam);
        return response.write(*out);
    } else if (code=="connectSim.1") {
        ConstString inParam = request.find("sim").asString();
        printf("Got %s. ",inParam.c_str());
        ConstString outParam;
        if (simConnected){
            printf("Disconnecting from robot simulator.\n");
            simDevice.close();
            simConnected = false;
            simPos = 0;
            simCart->close();
            delete simCart;
            simCart=0;
            // Maybe perform some checks here
            outParam = "SIMOFF";
        } else {
            printf("Connecting to robot simulator.\n");
            Property options;
            options.put("device","remote_controlboard");
            options.put("remote","/ravebot");
            options.put("local","/webLocal");
            simDevice.open(options);
            bool ok = true;        
            if(!simDevice.isValid()) {
                printf("[error] ravebot device not available.\n");
                ok = false;
            } else printf ("[success] ravebot device available.\n");
            if(!simDevice.view(simPos)) {
                printf("[error] ravebot simPos not available.\n");
                ok = false;
            } else printf ("[success] ravebot simPos available.\n");
            simCart = new CartesianClient;
            if(!simCart->open()) {
                printf("[error] cannot open ravebot simCart.\n");
                ok=false;
            } else printf ("[success] opened ravebot simCart.\n");
            if(!ok) {
                simDevice.close();
                simConnected = false;
                simPos = 0;
                if(simCart) {
                    simCart->close();
                    delete simCart;
                    simCart = 0;
                }
                outParam = "SIMOFF";
            } else {
                simConnected = true;
                outParam = "SIMON";
            }
        }
        response.addString(outParam);
        return response.write(*out);
    } else if (code=="index") {
        response.addString(readFile("index.html").c_str());
        return response.write(*out);
    } else if (code=="joint") {
        response.addString(readFile("joint.html").c_str());
        return response.write(*out);
    } else if (code=="joint.1") {
        ConstString theJoint = request.find("joint").asString();
        int inJoint = stringToInt(theJoint);
        ConstString inMovement = request.find("movement").asString();
        printf("Going to move joint [%d] towards the [%s].\n", inJoint, inMovement.c_str());
        if(simPos) simPos->setPositionMode();
        if((simPos)&&(inMovement == ConstString("right"))) simPos->relativeMove(inJoint-1,JOYPAD_RELMOVE);
        if((simPos)&&(inMovement == ConstString("left"))) simPos->relativeMove(inJoint-1,-JOYPAD_RELMOVE);
        if(realPos) realPos->setPositionMode();
        if((realPos)&&(inMovement == ConstString("right"))) realPos->relativeMove(inJoint-1,JOYPAD_RELMOVE);
        if((realPos)&&(inMovement == ConstString("left"))) realPos->relativeMove(inJoint-1,-JOYPAD_RELMOVE);
        return response.write(*out);
    } else if (code=="joint.2") {
        ConstString inMovement = request.find("movement").asString();
        ConstString q1 = request.find("one").asString();
        ConstString q2 = request.find("two").asString();
        ConstString q3 = request.find("three").asString();
        ConstString q4 = request.find("four").asString();
        ConstString q5 = request.find("five").asString();
        double targets[5];
        targets[0] = stringToDouble(q1);
        targets[1] = stringToDouble(q2);
        targets[2] = stringToDouble(q3);
        targets[3] = stringToDouble(q4);
        targets[4] = stringToDouble(q5);
        printf("Going to move%s\n", inMovement.c_str());
        if(simPos) simPos->setPositionMode();
        if((simPos)&&(inMovement == ConstString("absolute"))) simPos->positionMove(targets);
        if((simPos)&&(inMovement == ConstString("relative"))) simPos->relativeMove(targets);
        if(realPos) realPos->setPositionMode();
        if((realPos)&&(inMovement == ConstString("absolute"))) realPos->positionMove(targets);
        if((realPos)&&(inMovement == ConstString("relative"))) realPos->relativeMove(targets);
        return response.write(*out);
    } else if (code=="stop.0") {
        if(simPos) simPos->stop();
        if(realPos) realPos->stop();
        return response.write(*out);
    } else if (code=="cartesian") {
        response.addString(readFile("cartesian.html").c_str());
        return response.write(*out);
    } else if (code=="cartesian.1") {
        ConstString theAxis = request.find("axis").asString();
        ConstString inMovement = request.find("movement").asString();
        printf("Going to move axis [%s] towards the [%s].\n", theAxis.c_str(), inMovement.c_str());
        double cartCoords[5];
        if(simCart) simCart->stat(cartCoords);
        if(realCart) simCart->stat(cartCoords); // REAL OVERWRITES COORDS
        printf("At: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
        if(inMovement == ConstString("right")) {
            printf("right movement...\n");
            if(theAxis == ConstString("px")) cartCoords[0]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("py")) cartCoords[1]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("pz")) cartCoords[2]+= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("oyP")) cartCoords[3]+= CJOYPAD_RELMOVE_ORI;
            if(theAxis == ConstString("ozPP")) cartCoords[4]+= CJOYPAD_RELMOVE_ORI;
        } else if(inMovement == ConstString("left")) {
            if(theAxis == ConstString("px")) cartCoords[0]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("py")) cartCoords[1]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("pz")) cartCoords[2]-= CJOYPAD_RELMOVE_POS;
            if(theAxis == ConstString("oyP")) cartCoords[3]-= CJOYPAD_RELMOVE_ORI;
            if(theAxis == ConstString("ozPP")) cartCoords[4]-= CJOYPAD_RELMOVE_ORI;
        }
        printf("To: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
        if(simCart) simCart->movl(cartCoords);
        if(realCart) realCart->movl(cartCoords);
        return response.write(*out);
    } else if (code=="cstop.0") {
        if(simCart) simCart->stop();
        if(realCart) realPos->stop();
        return response.write(*out);
    } else if (code=="cartesian.2") {
        ConstString origin = request.find("origin").asString();
        ConstString movement = request.find("movement").asString();
        ConstString px = request.find("px").asString();
        ConstString py = request.find("py").asString();
        ConstString pz = request.find("pz").asString();
        ConstString oyP = request.find("oyP").asString();
        ConstString ozPP = request.find("ozPP").asString();
        double targets[5];
        targets[0] = stringToDouble(px);
        targets[1] = stringToDouble(py);
        targets[2] = stringToDouble(pz);
        targets[3] = stringToDouble(oyP);
        targets[4] = stringToDouble(ozPP);
        printf("Going to move%s\n", origin.c_str());
        if(origin == ConstString("abs_base")) {
            if(movement == ConstString("movj")) {
                if(simCart) simCart->movj(targets);
                if(realCart) realCart->movj(targets);
            }else if(movement == ConstString("movl")) {
                if(simCart) simCart->movl(targets);
                if(realCart) realCart->movl(targets);
            }
        } else if (origin == ConstString("rel_base")) {
            double cartCoords[5];
            if(simCart) simCart->stat(cartCoords);
            if(realCart) simCart->stat(cartCoords); // REAL OVERWRITES COORDS
            printf("At: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
            cartCoords[0]+= targets[0];
            cartCoords[1]+= targets[1];
            cartCoords[2]+= targets[2];
            cartCoords[3]+= targets[3];
            cartCoords[4]+= targets[4];
            printf("To: %f %f %f %f %f\n",cartCoords[0],cartCoords[1],cartCoords[2],cartCoords[3],cartCoords[4]);
            if(movement == ConstString("movj")) {
                if(simCart) simCart->movj(cartCoords);
                if(realCart) realCart->movj(cartCoords);
            }else if(movement == ConstString("movl")) {
                if(simCart) simCart->movl(cartCoords);
                if(realCart) realCart->movl(cartCoords);
            }
        }
        return response.write(*out);
    } else if (code=="video") {
        ConstString camHost = Network::queryName("/ravebot/img:o").getHost();
        int camPort = Network::queryName("/ravebot/img:o").getPort();
//        printf("simCam running at: http://%s:%d\n",camHost.c_str(),camPort);
        ConstString camSocket = "http://";
        camSocket += camHost + ":" + ConstString::toString(camPort) + "/?action";
        printf("\nCam running at: %s\n\n", camSocket.c_str());
        string str = readFile("video.html");
        replaceAll(str, "<SIMCAMIP>", camSocket.c_str());
        response.addString(str.c_str());
        return response.write(*out);
    }

    ConstString prefix = "<html>\n<head>\n<title>YARP web test</title>\n";
    prefix += "<link href=\"style.css\" media=\"screen\" rel=\"stylesheet\" type=\"text/css\" />\n";
    prefix += "</head>\n<body>\n";

    if (code=="push") {
        prefix += "<h1>Counter count</h1>\n";
        prefix += "<div>(<a href='/test'>back</a>)</div>\n";

        response.addString(prefix);
        response.addString("stream");
        response.addInt(1);
        return response.write(*out);
    }

    ConstString postfix = "</body>\n</html>";

    ConstString txt = prefix;
    txt += ConstString("<h1>") + code + "</h1>\n";
    txt += "<div>Is this working for you? <a href='/yes'>yes</a> <a href='/no'>no</a></div>\n";
    if (!request.check("day")) {
        txt += "<div>By the way, what day is it?</div>\n<form><input type='text' id='day' name='day' value='Sunday' /><input type='submit' value='tell me' /></form>\n";
    } else {
        txt += ConstString("<div>So today is ") + request.find("day").asString() + ", is it? Hmm. I don't think I'm going to bother remembering that.</div>\n";
    }
    txt += "<div><a href='/push'>How many counter counts?</a> (streaming example)</div>\n";
    txt += postfix;

    response.addString(txt);
    return response.write(*out);
}

