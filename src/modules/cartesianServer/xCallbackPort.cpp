
#include "xCallbackPort.h"

/************************************************************************/

void xCallbackPort::onRead(Bottle& b) {
    printf("[xCallbackPort] Got %s\n", b.toString().c_str());
}
