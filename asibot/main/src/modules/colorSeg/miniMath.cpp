// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <miniMath.h>

Punto::Punto(double ix = 0,double iy = 0) : x(ix), y(iy) {}

Punto::Punto(double im, double id, int) {
    x = id / sqrt(1+(im*im));
    y = im * x;
}

Punto Punto::operator+(Punto& ip) {		//binary operator
    return Punto(x + ip.x, y + ip.y);
}

Punto Punto::operator-(Punto& ip) {		//binary operator
    return Punto(x - ip.x, y - ip.y);
}

Punto Punto::operator-() {				//unary operator
    return Punto(-x, -y);
}


Recta::Recta(double im = 0,double ib = 0) : m(im), b(im) {}

Recta::Recta(Punto ip1,Punto ip2) {
    m = (ip2.y - ip1.y) / (ip2.x - ip1.x);
    b = ip1.y - (m * ip1.x);
}

Recta::Recta(Punto ip,double im) {
    m = im;
    b = ip.y - (im * ip.x);
}

double distPP(Punto ip1, Punto ip2)
{
    return sqrt(pow((ip1.x-ip2.x),2)+pow((ip1.y-ip2.y),2));
}

Punto cruceRR(Recta ir1, Recta ir2)
{
    Punto cruce;
    cruce.x = (ir2.b - ir1.b) / (ir1.m - ir2.m);
    cruce.y = (ir1.m * cruce.x) + ir1.b;
    return cruce;
}

double perp(Recta ir) { // preferred use
    return -(1/ir.m);
}

/*double distPR(Punto ip, Recta ir)
{
	return distPP(cruceRR(ir,Recta(ip,ir.perp())),ip);
};

Punto PsobreR(Punto ip, Recta ir, double idist){
	return ip;
};
*/

//#endif

