// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 *
 * Universidad Carlos III de Madrid (C) 2011
 *
 * Author: Juan G Victores
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 */

#ifndef __PXTOREAL_H__
#define __PXTOREAL_H__

#include <yarp/sig/Matrix.h>
#include <yarp/os/all.h>

#include "miniMath.h"

using namespace yarp::sig;

class pxToReal {
  public:
    pxToReal();

    //Esta función identifica en qué región se encuentra la lata
    bool localizar(const double centroidx, const double centroidy, double& outx, double& outy);

    void printmatrix(Matrix& inmatrix);
  private:
    int h,j; //filas,columnas
    Matrix realx;
    Matrix realy;
    Matrix imrx;
    Matrix imry;
};


#endif

