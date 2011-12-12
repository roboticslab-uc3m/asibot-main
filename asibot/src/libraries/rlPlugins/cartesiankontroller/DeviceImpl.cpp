
// ----------------------------------------------------------------------------

#include "CartesianKontroller.h"

// ----------------------------------------------------------------------------


bool CartesianKontroller::open(Searchable &config) {

    printf("[device] ===================================\n");
    printf("[device] setting our local device parameters\n");

    Value v = config.check("DOF",Value(DEFAULT_NUM_MOTORS));
    num_motors = v.asInt();
    if (num_motors > MAX_NUM_MOTORS) {
        printf("[device] exceeded max DOF, setting default \n");
        num_motors=MAX_NUM_MOTORS;
    }
    printf("[device] using %d DOF\n",v.asInt());

    printf("[device] =======================================\n");
    printf("[device] check our remote device can be accessed\n");

    Property options;

    options.put("device","remote_controlboard");

    Value remote = config.check("remote",Value("/ravebot"));
    options.put("remote",remote);

    Value local = config.check("remote",Value("/ravebot"));
    ConstString prefixName="/cartesiankontroller";
    prefixName = prefixName + local.toString();
    options.put("local",prefixName);

    dd.open(options);

    if (!dd.isValid()) {
        printf("[device] [error] remote_controlboard not valid\n\n");
        return false;
    }

    dd.view(pos);
    pos->getAxes(&num_motors);
    printf("[device] going to control %d motors\n",num_motors);

    dd.view(enc);
    dd.view(vel);

//    vel->setVelocityMode();

    this->start();

    return true;
}

bool CartesianKontroller::close() {
//    pos->setPositionMode();

    printf("[device] ========== CLOSE ===============\n");
    return true;
}
