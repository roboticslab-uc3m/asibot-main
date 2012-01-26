// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TRAJGEN_H__
#define __TRAJGEN_H__

#include <stdio.h>

class OrderThreeTraj {
  public:
    OrderThreeTraj() {}
    void configure(const double xi, const double xf, const double _T) {
        T = _T;
        a0 = xi;
        a1 = 0;
        a3 = 2*(xi-xf)/(T*T*T);
        a2 = (xf-xi)/(T*T) - a3*T;
    }
    void configure(const double xi, const double xf, const double xdi, const double xdf, const double _T) {
        T = _T;
        a0 = xi;
        a1 = xdi;
        a3 = (xdf-2*xdi)/(T*T) + 2*(xi-xf)/(T*T*T);
        a2 = (xf-xi-xdi*T)/(T*T) - a3*T;
    }
    double get(const double ti) {
        if (ti>T) return a3*T*T*T + a2*T*T + a1*T + a0;  // Security hack
        return a3*ti*ti*ti + a2*ti*ti + a1*ti + a0;
    }
    double getd(const double ti) {
        if (ti>T) return 3*a3*T*T + 2*a2*T + a1;  // Security hack
        return 3*a3*ti*ti + 2*a2*ti + a1;
    }
    double getdd(const double ti) { 
        if (ti>T) return 6*a3*T + 2*a2;  // Security hack
        return 6*a3*ti + 2*a2;
    }
    bool maxVelBelow(const double thresVel) { 
        return (getd(T/2)<thresVel);
    }
    bool maxAccBelow(const double thresAcc) { 
        return (getdd(T/2)<thresAcc);
    }
    double getT() {
        return T;
    }
  private:
    double a3,a2,a1,a0,T;
};

#endif

