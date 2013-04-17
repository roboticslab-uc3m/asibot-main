// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TinyMath.hpp"

// ----------------------------------------------------------------------------

double toDeg(const double inRad) {
    return (inRad * 180.0 / M_PI);  // return (inRad * 180.0 / 3.14159265);
}

// ----------------------------------------------------------------------------

double toRad(const double inDeg) {
    return (inDeg * M_PI / 180.0);  // return (inDeg * 3.14159265 / 180.0);
}

// ----------------------------------------------------------------------------

yarp::sig::Matrix axis2dcm(const yarp::sig::Vector &v, unsigned int verbose) {

    if (v.length()<4) {
        if (verbose) fprintf(stderr,"axis2dcm() failed\n");
        return yarp::sig::Matrix(0,0);
    }

    yarp::sig::Matrix R = eye(4,4);

    double theta=v[3];
    if (theta==0.0) return R;

    double c=cos(theta);
    double s=sin(theta);
    double C=1.0-c;

    double xs =v[0]*s;
    double ys =v[1]*s;
    double zs =v[2]*s;
    double xC =v[0]*C;
    double yC =v[1]*C;
    double zC =v[2]*C;
    double xyC=v[0]*yC;
    double yzC=v[1]*zC;
    double zxC=v[2]*xC;
    
    R(0,0)=v[0]*xC+c;
    R(0,1)=xyC-zs;
    R(0,2)=zxC+ys;
    R(1,0)=xyC+zs;
    R(1,1)=v[1]*yC+c;
    R(1,2)=yzC-xs;
    R(2,0)=zxC-ys;
    R(2,1)=yzC+xs;
    R(2,2)=v[2]*zC+c;

    return R;
}

// ----------------------------------------------------------------------------

