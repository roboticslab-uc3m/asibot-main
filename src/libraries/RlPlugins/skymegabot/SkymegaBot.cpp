// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SkymegaBot.h"

// -----------------------------------------------------------------------------

bool SkymegaBot::open(Searchable& config) {
    char serialport[13] = "/dev/ttyUSB0";
    int baudrate = B9600;  // default
    char buf[256];
    int rc,n;

    fd = serialport_init(serialport, baudrate);
    if(!fd) {
        printf("NULL fd, bye!\n");
        ::exit(-1);
    }
    printf("[success] Skymegabot open(), fd: %d\n",fd);

    return true;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::getAxes(int *axes) {
    *axes = DEFAULT_NUM_MOTORS;
    return true;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::setVelocityMode() {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::velocityMove(int j, double sp) {
    printf("[SkymegaBot] velocityMove(%d, %f)\n",j,sp);
    if (j>DEFAULT_NUM_MOTORS) return false;
    unsigned char cmdByte = (unsigned int)sp/46;  // 3000/64 = 46; usamos 6 bits -> 64
    printf("cmdByte (pre-dir): 0x%x\n",cmdByte);
    unsigned char direccion = j;
    direccion <<= 6;  // shift 6 positions
    cmdByte |= direccion;
    printf("cmdByte (post-dir): 0x%x\n",cmdByte);
    int res = serialport_writebyte(fd, cmdByte);
    if(res==-1) return false;
    return true;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::velocityMove(const double *sp) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::setRefAcceleration(int j, double acc) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::setRefAccelerations(const double *accs) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::getRefAcceleration(int j, double *acc) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::getRefAccelerations(double *accs) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::stop(int j) {
    return false;
}

// -----------------------------------------------------------------------------

bool SkymegaBot::stop() {
    return false;
}

