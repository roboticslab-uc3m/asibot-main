// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TRAJGEN_H__
#define __TRAJGEN_H__

#include <stdio.h>

/**
 *
 * @ingroup OrderThreeTraj
 *
 * OrderThreeTraj generates a 1DOF order-three trajectory.
 *
 */
class OrderThreeTraj {
  public:
    OrderThreeTraj() {}
    /**
    * Configure the trajectory. Forces null initial and final velocities.
    */
    void configure(const double xi, const double xf, const double _T) {
        T = _T;
        a0 = xi;
        a1 = 0;
        a3 = 2*(xi-xf)/(T*T*T);
        a2 = (xf-xi)/(T*T) - a3*T;
    }

    /**
    * Configure the trajectory.
    */
    void configure(const double xi, const double xf, const double xdi, const double xdf, const double _T) {
        T = _T;
        a0 = xi;
        a1 = xdi;
        a3 = (xdf-2*xdi)/(T*T) + 2*(xi-xf)/(T*T*T);
        a2 = (xf-xi-xdi*T)/(T*T) - a3*T;
    }

    /**
    * Return the value of the function at instant ti.
    */
    double get(const double ti) {
        if (ti>T) return a3*T*T*T + a2*T*T + a1*T + a0;  // Security hack
        return a3*ti*ti*ti + a2*ti*ti + a1*ti + a0;
    }

    /**
    * Return the value of the first derivative of the function at instant ti.
    */
    double getd(const double ti) {
        if (ti>T) return 3*a3*T*T + 2*a2*T + a1;  // Security hack
        return 3*a3*ti*ti + 2*a2*ti + a1;
    }

    /**
    * Return the value of the second derivative of the function at instant ti.
    */
    double getdd(const double ti) { 
        if (ti>T) return 6*a3*T + 2*a2;  // Security hack
        return 6*a3*ti + 2*a2;
    }

    /**
    * Check if the maximum of the first derivative is below a certain threshold.
    */
    bool maxVelBelow(const double thresVel) { 
        return (getd(T/2)<thresVel);
    }

    /**
    * Check if the maximum of the second derivative is below a certain threshold.
    */
    bool maxAccBelow(const double thresAcc) { 
        return (getdd(T/2)<thresAcc);
    }

    /**
    * @return duration assigned to the trajectory instance.
    */
    double getT() {
        return T;
    }
  private:
    double a3,a2,a1,a0,T;
};

#endif

