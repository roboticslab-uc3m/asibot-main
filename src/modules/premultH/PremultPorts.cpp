
#include "PremultPorts.hpp"

/************************************************************************/

void PremultPorts::setOutPort(Port* _outPort) {
    outPort = _outPort;
}

/************************************************************************/

void PremultPorts::setH(yarp::sig::Matrix* _H) {
    H = _H;
}

/************************************************************************/

void PremultPorts::onRead(Bottle& b) {
    printf("[PremultPorts] Got %s\n", b.toString().c_str());
    Bottle outB;
    for (int i=0; i<b.size(); i++) {  // Parse each element
        if(!b.get(i).isList()) {  // Check if element is a list
            fprintf(stderr,"[error] need list to parse\n");
            return;
        }
        Bottle* inListElement = b.get(i).asList();
        yarp::sig::Matrix originalH(4,4);
        originalH.eye();
        if(inListElement->size()==3) {  // case: inListElement is a point 3-vector
            originalH(0,3) = inListElement->get(0).asDouble();
            originalH(1,3) = inListElement->get(1).asDouble();
            originalH(2,3) = inListElement->get(2).asDouble();
            printf("PremultH using custom H:\n%s\n",H->toString().c_str());
            yarp::sig::Matrix endH = (*H) * originalH;
            Bottle outListElement;
            outListElement.addDouble(endH(0,3));
            outListElement.addDouble(endH(1,3));
            outListElement.addDouble(endH(2,3));
            outB.addList() = outListElement;
        } else {
            fprintf(stderr,"[error] for now only parsing 3-double lists\n");
            return;  // case: other --> still not implemented
        }
    }
    outPort->write(outB);
}

/************************************************************************/

