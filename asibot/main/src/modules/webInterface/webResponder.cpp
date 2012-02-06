// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "webResponder.h"

/************************************************************************/
bool WebResponder::setContextPath(const ConstString& _contextPath) {
    contextPath = _contextPath;
    return true;
}

/************************************************************************/
ConstString WebResponder::getCss() {

    return "\n\
body { background: black; color: white; } \n\
h1 { font-size: 300%; color: yellow; } \n\
div { padding-bottom: 10px; } \n\
";
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
    if (code=="css") {
        response.addString(getCss());
        response.addString("mime");
        response.addString("text/css");
        return response.write(*out);
    }

    ConstString prefix = "<html>\n<head>\n<title>YARP web test</title>\n";
    prefix += "<link href=\"/css\" media=\"screen\" rel=\"stylesheet\" type=\"text/css\" />\n";
    prefix += "</head>\n<body>\n";

    if (code=="push") {
        prefix += "<h1>Potato count</h1>\n";
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
    txt += "<div><a href='/push'>How many potatoes?</a> (streaming example)</div>\n";
    txt += postfix;

    response.addString(txt);
    return response.write(*out);
}

