// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//#ifndef __MINIMATH_H__
//#define __MINIMATH_H__

#include <math.h>

class Punto {
  public:
	double x, y;
	Punto(double ix, double iy);
	Punto(double im, double id, int);
	Punto operator+(Punto& ip);  // binary operator
	Punto operator-(Punto& ip);  // binary operator
	Punto operator-();  // unary operator
};

class Recta {
  public:
	double  m, b;
	Recta(double im, double ib);
	Recta(Punto ip1, Punto ip2);
	Recta(Punto ip,double im);
};

double distPP(Punto ip1, Punto ip2);
Punto cruceRR(Recta ir1, Recta ir2);

double perp(Recta ir); // preferred use

/*double distPR(Punto ip, Recta ir)
{
	return distPP(cruceRR(ir,Recta(ip,ir.perp())),ip);
};

Punto PsobreR(Punto ip, Recta ir, double idist){
	return ip;
};
*/

//#endif

