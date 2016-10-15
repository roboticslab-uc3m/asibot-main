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
    lastEditName="[none]";
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
bool WebResponder::setResourceFinder(ResourceFinder &rf) {
    this->rf = rf;
    return true;
}

/************************************************************************/
bool WebResponder::setUserPath(const ConstString& _userPath) {
    userPath = _userPath;
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
string WebResponder::readHtml(const ConstString& fileName) {
    ConstString filePath = rf.findFileByName(std::string("html/")+fileName);
    return readFile(filePath);
}

/************************************************************************/
string WebResponder::readFile(const ConstString& filePath) {
    printf("filePath: %s\n",filePath.c_str());
    // thank you Tyler McHenry @ nerdland.net and KeithB @ ndssl.vbi.vt.edu for this algorithm
    // link: http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring [2012-02-06]
    std::ifstream t(filePath.c_str());
    std::string str;
    if(!t.is_open()) {
        printf("Not able to open file.\n");
        return str;
    }
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
bool WebResponder::appendToFile(const ConstString& fileName, const ConstString& inString) {
    ConstString filePath = userPath + fileName;
    printf("saving: %s\n",inString.c_str());
    printf("to file: %s\n",filePath.c_str());
    std::ofstream t(filePath.c_str(), std::ios::app);
    t << inString << std::endl;
    t.close();
    return true;
}

/************************************************************************/
bool WebResponder::rewriteFile(const ConstString& fileName, const ConstString& inString) {
    ConstString filePath = userPath + fileName;
    printf("rewriting: %s\n",inString.c_str());
    printf("to file: %s\n",filePath.c_str());
    std::ofstream t(filePath.c_str());
    t << inString << std::endl;
    t.close();
    return true;
}

/************************************************************************/
bool WebResponder::deleteFile(const ConstString& absFile){ // needs absoulte path
    if (remove(absFile.c_str()) != 0 ) {
        printf("[error] could not delete file");
        return false;
    } else printf("Deleted %s",absFile.c_str());
    return true;
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
ConstString WebResponder::doubleToString(const double& inDouble) {
    // [thank you Adam Rosenfield] http://stackoverflow.com/questions/1123201/convert-double-to-string-c
    std::ostringstream s;
    s << inDouble;
    return ConstString(s.str().c_str());
}

/************************************************************************/
ConstString WebResponder::intToString(const int& inInt) {
    std::ostringstream s;
    s << inInt;
    return ConstString(s.str().c_str());
}

/************************************************************************/
ConstString WebResponder::pipedExec(const ConstString& cmd) {
    // http://stackoverflow.com/a/478960
    const int bufferSize = 128;
    char buffer[bufferSize];
    std::string result = "";
    FILE *(*ppopen)(const char *, const char *);
    int (*ppclose)(FILE *);
#ifdef WIN32
    ppopen = _popen;
    ppclose = _pclose;
#else
    ppopen = popen;
    ppclose = pclose;
#endif
    FILE* pipe = ppopen(cmd.c_str(), "r");
    if (!pipe) return "could not execute command, popen() failed";
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, bufferSize, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        ppclose(pipe);
        return "error, aborting command: " + cmd;
    }
    ppclose(pipe);
    return result;
}

/************************************************************************/
ConstString WebResponder::pointButtonCreator(const ConstString& pointsFile) {
    ConstString ret;
    printf("Reading points from file: %s\n",pointsFile.c_str());
    std::ifstream ifs(pointsFile.c_str());
    if (!ifs.is_open()) {
        printf("[warning] point file not open, assuming no points yet.\n");
        ret += "[No points captured yet]";
        return ret;
    }
    string line;
    while (getline(ifs, line)) {
        line += ' ';  // add a comma for easier parsing
        printf("line: %s.\n",line.c_str());
        int npos=0;
        int lpos=0;
        ConstString pointName;
        ConstString values;
        while ((npos = (int)line.find(' ', lpos)) != string::npos) {
            ConstString subs(line.substr(lpos, npos - lpos).c_str());
            if (lpos==0) pointName = subs;
            else {
                values += subs;
                values += ",";
            }
            lpos = npos+1;
        }
        // ret empty for now
        ret += "<button onClick=\"pointToText('";
        ret += pointName.c_str();
        ret += "','";
        ConstString tvalues = values.substr(0,values.length()-1);
        ret += tvalues.c_str();
        ret += "');\">";
        ret += pointName.c_str();
        ret += "</button><br>";
    }
    printf("Done reading points from file: %s\n",pointsFile.c_str());
    return ret;
}

/************************************************************************/
ConstString WebResponder::wordOptionCreator(const ConstString& wordsFile) {
    ConstString ret;
    printf("Reading words from file: %s\n",wordsFile.c_str());
    std::ifstream ifs(wordsFile.c_str());
    if (!ifs.is_open()) {
        printf("[warning] word file not open, assuming no words yet.\n");
        ret += "[No words captured yet]";
        return ret;
    }
    string line;
    while (getline(ifs, line)) {
        line += ' ';  // add a comma for easier parsing
        printf("line: %s.\n",line.c_str());
        int npos=0;
        int lpos=0;
        ConstString pointName;
        ConstString values;
        while ((npos = (int)line.find(' ', lpos)) != string::npos) {
            ConstString subs(line.substr(lpos, npos - lpos).c_str());
            if (lpos==0) pointName = subs;
            else {
                values += subs;
                values += ",";
            }
            lpos = npos+1;
        }
        // ret empty for now
        ret += "<option>";
        ret += line.c_str();
        ret += "</option>";
    }
    printf("Done reading words from file: %s\n",wordsFile.c_str());
    return ret;
}

#ifdef WIN32
ConstString WebResponder::fileListCreator() {
    ConstString ret;
    ConstString filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            string fileName(ffd.cFileName);
            if (fileName.size() > 3 && (int)fileName.rfind(".py") == fileName.size() - 3) {
                printf("[%s] was py\n", fileName.c_str());
                if ((fileName != "AsibotPy.py") && (fileName != "template.py")) {
                    ret += "<option>";
                    ret += fileName.substr(0, fileName.size() - 3).c_str();
                    ret += "</option>";
                }
            }
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else printf("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}

/************************************************************************/
ConstString WebResponder::taskListCreator() {
    ConstString ret;
    ConstString filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            string fileName(ffd.cFileName);
            if ((int)fileName.find(".task", 0) != string::npos) {
                printf("[%s] was task\n", fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size() - 5).c_str();
                ret += "</option>";
            }
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else printf("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}

/************************************************************************/
ConstString WebResponder::taskButtonCreator() {
    ConstString ret;
    ConstString filePath = userPath.substr(0, filePath.size() - 1) + "\\*";
    printf("Reading files from: %s\n", filePath.c_str());
    HANDLE hFind;
    WIN32_FIND_DATA ffd;
    if ((hFind = FindFirstFile(filePath.c_str(), &ffd)) != INVALID_HANDLE_VALUE) {
        do {
            string fileName(ffd.cFileName);
            int taskCount = 0;
            if ((int)fileName.find(".task", 0) != string::npos) {
                printf("[%s] was task, contents...\n", fileName.c_str());
                ConstString taskPath(userPath);
                taskPath += fileName.c_str();
                std::ifstream ifs(taskPath.c_str());
                if (!ifs.is_open()) {
                    printf("[warning] word file not open, assuming no words yet.\n");
                    ret += "[error]";
                    return ret;
                }
                string line;
                int lineCount = 0;
                string strProgram;
                string strSpeech;
                string strIcon;
                while (getline(ifs, line)) {
                    if (lineCount == 0) strProgram = line;
                    else if (lineCount == 1) strSpeech = line;
                    else if (lineCount == 2) strIcon = line;
                    lineCount++;
                }
                printf("Program: %s\nSpeech: %s\nIcon: %s\n", strProgram.c_str(), strSpeech.c_str(), strIcon.c_str());
                ret += "<button name='program' type='button' value='";
                ret += strProgram.c_str();
                ret += "'><img src='";
                ret += resourcePath;
                ret += "fig/";
                ret += strIcon.c_str();
                ret += "' height='200'></button>";
                //                printf("<button name='program' type='submit' value=");
                //                printf("%s", line.c_str());
                //                printf("</button>\n");
            }
            if (taskCount % 2 == 0) ret += " &nbsp;\n";
            else ret += "<br><br>\n";
            taskCount++;
        } while (FindNextFile(hFind, &ffd));
        FindClose(hFind);
    }
    else printf("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n", filePath.c_str());
    return ret;
}
#else

/************************************************************************/
ConstString WebResponder::fileListCreator() {
    ConstString ret;
    ConstString filePath = userPath;
    printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            string fileName(ep->d_name);
            if(fileName.size() > 3 && (int)fileName.rfind(".py") == fileName.size()-3) {
                printf("[%s] was py\n",fileName.c_str());
                if((fileName != "AsibotPy.py")&&(fileName != "template.py")) {
                    ret += "<option>";
                    ret += fileName.substr(0, fileName.size()-3).c_str();
                    ret += "</option>";
                }
            }
        }
       (void) closedir (dp);
    } else printf ("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}

/************************************************************************/
ConstString WebResponder::taskListCreator() {
    ConstString ret;
    ConstString filePath = userPath;
    printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            string fileName(ep->d_name);
            if((int)fileName.find(".task", 0) != string::npos) {
                printf("[%s] was task\n",fileName.c_str());
                ret += "<option>";
                ret += fileName.substr(0, fileName.size()-5).c_str();
                ret += "</option>";
            }
        }
       (void) closedir (dp);
    } else printf ("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}

/************************************************************************/
ConstString WebResponder::taskButtonCreator() {
    ConstString ret;
    ConstString filePath = userPath;
    printf("Reading files from: %s\n",filePath.c_str());
    DIR *dp;
    struct dirent *ep;
    dp = opendir(filePath.c_str());
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            string fileName(ep->d_name);
            int taskCount = 0;
            if((int)fileName.find(".task", 0) != string::npos) {
                printf("[%s] was task, contents...\n",fileName.c_str());
                ConstString taskPath(userPath);
                taskPath += fileName.c_str();
                std::ifstream ifs(taskPath.c_str());
                if (!ifs.is_open()) {
                    printf("[warning] word file not open, assuming no words yet.\n");
                    ret += "[error]";
                    return ret;
                }
                string line;
                int lineCount = 0;
                string strProgram;
                string strSpeech;
                string strIcon;
                while (getline(ifs, line)) {
                    if (lineCount==0) strProgram = line;
                    else if (lineCount==1) strSpeech = line;
                    else if (lineCount==2) strIcon = line;
                    lineCount++;
                }
                printf("Program: %s\nSpeech: %s\nIcon: %s\n",strProgram.c_str(),strSpeech.c_str(),strIcon.c_str());
                ret += "<button name='program' type='button' value='";
                ret += strProgram.c_str();
                ret += "'><img src='";
                ret += resourcePath;
                ret += "fig/";
                ret += strIcon.c_str();
                ret += "' height='200'></button>";
//                printf("<button name='program' type='submit' value=");
//                printf("%s", line.c_str());
//                printf("</button>\n");
            }
            if (taskCount%2==0) ret += " &nbsp;\n";
            else ret += "<br><br>\n";
            taskCount++;
        }
        (void) closedir (dp);
    } else printf ("[warning] Couldn't open the directory\n");
    printf("Done reading files from: %s\n",filePath.c_str());
    return ret;
}
#endif

/************************************************************************/
ConstString WebResponder::getCss() {
    return ConstString(readHtml("style.css").c_str());
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
        response.addString(readHtml("xmlhttp.js").c_str());
        return response.write(*out);
    } else if (code=="connectionTab.js") {
        response.addString(readHtml("connectionTab.js").c_str());
        return response.write(*out);
    } else if (code=="testEqual") {
        response.addString(readHtml("testEqual.html").c_str());
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
            if(!simCart->open("/ravebot")) {
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
        response.addString(readHtml("index.html").c_str());
        return response.write(*out);
    } else if (code=="joint") {
        string str = readHtml("joint.html");
        //TCbegin
        ConstString taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<JOPTS>", "<script src='jTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
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
        string str = readHtml("cartesian.html");
        //TCbegin
        ConstString taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<COPTS>", "<script src='cTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
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
        ConstString camHost = Network::queryName("/ravebot/asibot_tip/img:o").getHost();
        int camPort = Network::queryName("/ravebot/asibot_tip/img:o").getPort();
        ConstString camSocket = "http://";
        camSocket += camHost + ":" + intToString(camPort) + "/?action";
        printf("\nCam running at: %s\n\n", camSocket.c_str());
        string str = readHtml("video.html");
        replaceAll(str, "<SIMCAMIP>", camSocket.c_str());
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="capture.0") {
        if(simCart) simCart->stat(captureX);
        if(realCart) simCart->stat(captureX); // REAL OVERWRITES COORDS
        printf("At: %f %f %f %f %f\n",captureX[0],captureX[1],captureX[2],captureX[3],captureX[4]);
        ConstString coords("x=");
        coords += doubleToString(captureX[0]) + " y=";
        coords += doubleToString(captureX[1]) + " z=";
        coords += doubleToString(captureX[2]) + " rot(y')=";
        coords += doubleToString(captureX[3]) + " rot(z'')=";
        coords += doubleToString(captureX[4]);
        response.addString(coords.c_str());
        printf("Writing: %s\n",coords.c_str());
        return response.write(*out);
    } else if (code=="capture.1") {
        ConstString pname = request.find("pname").asString();
        printf("capture.1 saving capture.0 captures: %f %f %f %f %f\n",captureX[0],captureX[1],captureX[2],captureX[3],captureX[4]);
        ConstString captureStr(pname);
        captureStr += " ";
        captureStr += doubleToString(captureX[0]) + " ";
        captureStr += doubleToString(captureX[1]) + " ";
        captureStr += doubleToString(captureX[2]) + " ";
        captureStr += doubleToString(captureX[3]) + " ";
        captureStr += doubleToString(captureX[4]);
        appendToFile("points.ini",captureStr);
        response.addString(pname);
        return response.write(*out);
    } else if (code=="program") {
        string str = readHtml("program.html");

        //TCbegin
        ConstString taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<POPTS>", "<script src='pTaskTab.js' type='text/javascript'></script>");
        }
        //TCend

        replaceAll(str, "<FNAME>", lastEditName.c_str());

        ConstString fileList = fileListCreator();
        replaceAll(str, "<CARGARFICHEROS>", fileList.c_str());

        ConstString pointsFile = userPath + "points.ini";
        ConstString pointsButtons = pointButtonCreator(pointsFile);
        replaceAll(str, "<POINTS>", pointsButtons.c_str());

        ConstString editFile = userPath + lastEditName + ".py";
        string contents = readFile(editFile);
        replaceAll(str, "<CENTRALPIECE>", contents.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="create.0") {
        ConstString nfile = request.find("nfile").asString();
        lastEditName = nfile;
        nfile += ".py";
        ConstString templatePath = rf.findFileByName(string("user/") + "template.py");
        string str = readFile(templatePath);
        appendToFile(nfile,str);
        printf("create.0 %s file.\n",nfile.c_str());
        response.addString(str);
        return response.write(*out);
    } else if (code=="delete.0") {
        ConstString dfile = userPath;
        dfile += request.find("dfile").asString();
        response.addString(request.find("dfile").asString());
        dfile += ".py";
        deleteFile(dfile);
        printf("delete.0 %s file.\n",dfile.c_str());
        return response.write(*out);
    } else if (code=="delete.1") {
        ConstString dfile = userPath;
        dfile += request.find("dfile").asString();
        response.addString(request.find("dfile").asString());
        dfile += ".task";
        deleteFile(dfile);
        printf("delete.0 %s file.\n",dfile.c_str());
        return response.write(*out);
    } else if (code=="edit.0") {
        ConstString efile = userPath;
        efile += request.find("efile").asString();
        efile += ".py";
        printf("edit.0 %s file.\n",efile.c_str());
        string str = readFile(efile);
        lastEditName = request.find("efile").asString();
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="save.0") {
        ConstString sfile = request.find("sfile").asString();
        response.addString(request.find("sfile").asString());
        sfile += ".py";
        printf("save.0 %s file.\n",sfile.c_str());
        string lstr = request.find("lstr").asString().c_str();
        replaceAll(lstr, "<br>", "\n");
        replaceAll(lstr, "<equal>", "=");
        replaceAll(lstr, "<numsign>", "#");
        rewriteFile(sfile,lstr.c_str());
        return response.write(*out);
    } else if (code=="compile.0") {
        ConstString program = request.find("program").asString();
        printf("compile %s program.\n",program.c_str());
        ConstString cmd("python -m py_compile ");
        cmd += userPath + program + ".py";
        cmd += " 2>&1"; // redirect stderr to stdout (see py_compile module)
        ConstString res = pipedExec(cmd);
        res.empty() ? response.clear() : response.addString(res);
        return response.write(*out);
    } else if (code=="speech") {
        string str = readHtml("speech.html");
        //TCbegin
        ConstString taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<SOPTS>", "<script src='sTaskTab.js' type='text/javascript'></script>");
        }
        //TCend
        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="speech.0") {
        ConstString word = request.find("word").asString();
        appendToFile("words.ini",word);
        response.addString(word.c_str());
        return response.write(*out);
    } else if (code=="launcher") {
        string str = readHtml("launcher.html");

        ConstString taskList = taskButtonCreator();
        replaceAll(str, "<BOTONESTAREAS>", taskList.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="assigner") {
        string str = readHtml("assigner.html");

        //TCbegin
        ConstString taskcreator = request.find("taskcreator").asString();
        if(taskcreator=="on"){
            replaceAll(str, "<AOPTS>", "<script src='aTaskTab.js' type='text/javascript'></script>");
        }
        //TCend

        ConstString fileList = fileListCreator();
        replaceAll(str, "<CARGARFICHEROS>", fileList.c_str());

        ConstString wordsFile = userPath + "words.ini";
        ConstString wordOptions = wordOptionCreator(wordsFile);
        replaceAll(str, "<WORDS>", wordOptions.c_str());

        ConstString taskList = taskListCreator();
        replaceAll(str, "<CARGARTAREAS>", taskList.c_str());

        response.addString(str.c_str());
        return response.write(*out);
    } else if (code=="docking") {
        response.addString(readHtml("docking.html").c_str());
        return response.write(*out);
    } else if (code=="jTaskTab.js") {
        response.addString(readHtml("jTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="cTaskTab.js") {
        response.addString(readHtml("cTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="pTaskTab.js") {
        response.addString(readHtml("pTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="aTaskTab.js") {
        response.addString(readHtml("aTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="sTaskTab.js") {
        response.addString(readHtml("sTaskTab.js").c_str());
        return response.write(*out);
    } else if (code=="assigner.0") {
        ConstString tname = request.find("tname").asString();
        response.addString(tname);
        tname += ".task";
        printf("assign.0 %s file.\n",tname.c_str());
        ConstString pfile = request.find("pfile").asString();
        ConstString swords = request.find("swords").asString();
        ConstString iname = request.find("iname").asString();
        ConstString lstr(pfile);
        lstr += "\n";
        lstr += swords + "\n";
        lstr += iname + "\n";
        rewriteFile(tname,lstr.c_str());
        return response.write(*out);
    } else if (code=="launch.0") {
        ConstString program = request.find("program").asString();
        printf("execute %s program.\n",program.c_str());
        response.addString(program.c_str());
        ConstString programPath = userPath + program;
        ConstString cmd("python ");
        cmd += programPath;
        cmd += ".py";
        int i=system (cmd.c_str());
        printf ("The value returned was: %d.\n",i);
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

