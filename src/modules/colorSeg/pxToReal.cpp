// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "pxToReal.h"

pxToReal::pxToReal() {
    ///////////////////////////////// patron_lata_roja ///////////////////////////////
    h=8; // rows,filas
    j=8; // columns,columnas
    realx.resize(h,j);
    realy.resize(h,j);
    imrx.resize(h,j);
    imry.resize(h,j);
    //coordenada X
    Vector fillCol(j);
    fillCol=15.0;
    realx.setCol(0,fillCol);
    fillCol=20.0;
    realx.setCol(1,fillCol);
    fillCol=25.0;
    realx.setCol(2,fillCol);
    fillCol=30.0;
    realx.setCol(3,fillCol);
    fillCol=35.0;
    realx.setCol(4,fillCol);
    fillCol=40.0;
    realx.setCol(5,fillCol);
    fillCol=45.0;
    realx.setCol(6,fillCol);
    fillCol=50.0;
    realx.setCol(7,fillCol);
    printmatrix(realx);

    //coordenada Y
    Vector fillRow(h);
    fillRow=130.0;
    realy.setRow(0,fillRow);
    fillRow=125.0;
    realy.setRow(1,fillRow);
    fillRow=120.0;
    realy.setRow(2,fillRow);
    fillRow=115.0;
    realy.setRow(3,fillRow);
    fillRow=110.0;
    realy.setRow(4,fillRow);
    fillRow=105.0;
    realy.setRow(5,fillRow);
    fillRow=100.0;
    realy.setRow(6,fillRow);
    fillRow=95.0;
    realy.setRow(7,fillRow);
    printmatrix(realy);

    //IMAGINARIAS
    imrx(h-8,0)=276.1970;
    imry(h-8,0)=444.7845; // 15 130
    imrx(h-8,1)=313.7091;
    imry(h-8,1)=443.8905;
    imrx(h-8,2)=353.0489;
    imry(h-8,2)=443.0091;
    imrx(h-8,3)=391.2626;
    imry(h-8,3)=441.4949;
    imrx(h-8,4)=431.7945;
    imry(h-8,4)=441.1091;
    imrx(h-8,5)=468.9617;
    imry(h-8,5)=440.8459;
    imrx(h-8,6)=505.5381;
    imry(h-8,6)=439.9064;
    imrx(h-8,7)=542.6381;
    imry(h-8,7)=439.8145; // 50 130

    imrx(h-7,0)=278.6825;
    imry(h-7,0)=457.8685;
    imrx(h-7,1)=317.6677;
    imry(h-7,1)=455.4955;
    imrx(h-7,2)=358.7564;
    imry(h-7,2)=455.9671;
    imrx(h-7,3)=397.3783;
    imry(h-7,3)=454.3338;
    imrx(h-7,4)=436.5285;
    imry(h-7,4)=453.5354;
    imrx(h-7,5)=477.8948;
    imry(h-7,5)=452.6848;
    imrx(h-7,6)=516.9862;
    imry(h-7,6)=452.2150;
    imrx(h-7,7)=553.1226;
    imry(h-7,7)=452.1608;

    imrx(h-6,0)=281.3173;
    imry(h-6,0)=470.5186;
    imrx(h-6,1)=323.6959;
    imry(h-6,1)=470.2399;
    imrx(h-6,2)=364.1865;
    imry(h-6,2)=468.6170;
    imrx(h-6,3)=406.4256;
    imry(h-6,3)=468.5610;
    imrx(h-6,4)=445.5657;
    imry(h-6,4)=466.9975;
    imrx(h-6,5)=487.1361;
    imry(h-6,5)=465.3488;
    imrx(h-6,6)=528.5487;
    imry(h-6,6)=464.9742;
    imrx(h-6,7)=567.0304;
    imry(h-6,7)=463.5969;

    imrx(h-5,0)=283.5813;
    imry(h-5,0)=484.3551;
    imrx(h-5,1)=328.0417;
    imry(h-5,1)=482.8676;
    imrx(h-5,2)=370.3201;
    imry(h-5,2)=482.9604;
    imrx(h-5,3)=412.6590;
    imry(h-5,3)=481.1201;
    imrx(h-5,4)=455.7398;
    imry(h-5,4)=480.8638;
    imrx(h-5,5)=497.4521;
    imry(h-5,5)=479.0402;
    imrx(h-5,6)=540.0958;
    imry(h-5,6)=477.8969;
    imrx(h-5,7)=581.0007;
    imry(h-5,7)=477.7460;

    imrx(h-4,0)=287.0997;
    imry(h-4,0)=501.1349;
    imrx(h-4,1)=332.1042;
    imry(h-4,1)=500.8027;
    imrx(h-4,2)=376.1090;
    imry(h-4,2)=499.3155;
    imrx(h-4,3)=420.8066;
    imry(h-4,3)=497.2429;
    imrx(h-4,4)=465.4274;
    imry(h-4,4)=496.0574;
    imrx(h-4,5)=509.3124;
    imry(h-4,5)=493.8038;
    imrx(h-4,6)=553.8662;
    imry(h-4,6)=493.8031;
    imrx(h-4,7)=597.1623;
    imry(h-4,7)=492.2388;

    imrx(h-3,0)=290.8147;
    imry(h-3,0)=519.1246;
    imrx(h-3,1)=337.4483;
    imry(h-3,1)=518.3349;
    imrx(h-3,2)=384.2935;
    imry(h-3,2)=516.0199;
    imrx(h-3,3)=430.1147;
    imry(h-3,3)=513.7739;
    imrx(h-3,4)=477.1953;
    imry(h-3,4)=512.5129;
    imrx(h-3,5)=523.2661;
    imry(h-3,5)=510.4840;
    imrx(h-3,6)=569.0143;
    imry(h-3,6)=509.7732;
    imrx(h-3,7)=611.8589;
    imry(h-3,7)=508.5551;

    imrx(h-2,0)=292.6244;
    imry(h-2,0)=537.3377;
    imrx(h-2,1)=342.0042;
    imry(h-2,1)=535.3465;
    imrx(h-2,2)=392.5372;
    imry(h-2,2)=534.3754;
    imrx(h-2,3)=443.0288;
    imry(h-2,3)=532.7271;
    imrx(h-2,4)=488.7916;
    imry(h-2,4)=531.1935;
    imrx(h-2,5)=536.5134;
    imry(h-2,5)=529.3426;
    imrx(h-2,6)=584.4109;
    imry(h-2,6)=527.3913;
    imrx(h-2,7)=629.1968;
    imry(h-2,7)=526.9037;

    imrx(h-1,0)=300.8160;
    imry(h-1,0)=561.0279; // 15 95
    imrx(h-1,1)=351.2385;
    imry(h-1,1)=559.2326;
    imrx(h-1,2)=400.2623;
    imry(h-1,2)=556.4254;
    imrx(h-1,3)=451.9441;
    imry(h-1,3)=554.3318;
    imrx(h-1,4)=502.5788;
    imry(h-1,4)=551.5919;
    imrx(h-1,5)=552.2883;
    imry(h-1,5)=549.0543;
    imrx(h-1,6)=600.8892;
    imry(h-1,6)=548.1629;
    imrx(h-1,7)=649.5412;
    imry(h-1,7)=545.4460; // 50 95

    printmatrix(imrx);
    printmatrix(imry);

}


void pxToReal::printmatrix(Matrix& inmatrix) {
    printf("%s\n\n",inmatrix.toString().c_str());
}


bool pxToReal::localizar(const double centroidx, const double centroidy, double& outx, double& outy) {
    ////////////////////////////////////// localizar ///////////////////////////////////////////
    Punto Centroide(centroidx,centroidy);
    Matrix my(1,h);
    Matrix Dx(1,j);
    Matrix Dy(h,j);
    Matrix dmy(1,j);
    dmy=500;

    // printf("check2\n");

    for (int i=0; i<j; i++) {
        for (int k=0; k<h; k++) {
            Dy(i,k) = centroidy-imry(i,k);
            if ((Dy(i,k) >= 0) && (Dy(i,k) < dmy(0,k) ) ) {
                // printf("check2a(%d,%d)\n",i,k);
                dmy(0,k)=Dy(i,k);
                // printf("check2b(%d,%d)\n",i,k);
                my(0,k)=i+1; /////////////OJO quite el +1
                // printf("check2c(%d,%d)\n",i,k);
                Dx(0,k)=(centroidx-imrx(my(0,k)-1,k));
                // printf("check2d(%d,%d)\n",i,k);
            }
        }
    }

    // printf("check3\n");

    int n=0;
    for (int k=0; k<j; k++) {
        if (Dx(0,k)>0) n=n+1; // puse -1 en lugar de 0
    }
//    if (n==-1) // REVISAR en comp con MATLAB version
//          n=0;

    //printf("check4\nDy:\n");
    //printmatrix(Dy);
    //printf("\ndmy:\n");
    //printmatrix(dmy);
    //printf("\nmy:\n");
    //printmatrix(my);
    //printf("\nDx:\n");
    //printmatrix(Dx);
    n--;
    int m = int(my(0,n));
    m--;
    printf("n = %d\tm = %d\n", n+1, m+1);

    // Range checks to prevent segfaults
    if((m<0)||(n<0)||(m>=7)||(n>7))
        return false;

    Recta Rec3(Punto(imrx(m,n),imry(m,n)),Punto(imrx(m+1,n),imry(m+1,n)));

    if (Rec3.m > 0) {  // Punto de fuga a la izquierda
        if (centroidx < imrx(m+1,n)) {  // Comprobación columna
            Recta Rec4(Punto(imrx(m,n),imry(m,n)),Centroide);
            if (Rec3.m < Rec4.m) {
                n=n-1;
            }
        }

        // Range checks to prevent segfaults
        if((m<-1)||(n<0)||(m>=7)||(n>=7))
            return false;

        if (Centroide.y > imry(m+1,n+1)) {  // Comprobación fila
            Recta Rec1(Punto(imrx(m+1,n+1),imry(m+1,n+1)),Punto(imrx(m+1,n),imry(m+1,n)));
            Recta Rec2(Punto(imrx(m+1,n+1),imry(m+1,n+1)),Centroide);
            if (Rec1.m > Rec2.m) {
                m=m+1;
            }
        }
    } else {

        // Range checks to prevent segfaults
        if((m<0)||(n<-1)||(m>=7)||(n>=7))
            return false;

        if (Centroide.x > imrx(m+1,n+1)) {  // Comprobación columna
            Recta Rec7(Punto(imrx(m+1,n+1),imry(m+1,n+1)),Punto(imrx(m,n+1),imry(m,n+1)));
            Recta Rec8(Punto(imrx(m+1,n+1),imry(m+1,n+1)),Centroide);
            if (Rec7.m < Rec8.m) {
                n=n+1;
            }
        }

        // Range checks to prevent segfaults
        if((m<0)||(n<0)||(m>7)||(n>=7))
            return false;

        if (Centroide.y < imry(m,n+1)) {  // Comprobación fila
            Recta Rec5(Punto(imrx(m,n),imry(m,n)),Punto(imrx(m,n+1),imry(m,n+1)));
            Recta Rec6(Punto(imrx(m,n),imry(m,n)),Centroide);
            if (Rec5.m > Rec6.m) {
                m=m-1;
            }
        }
    }

    // Range checks to prevent segfaults
    if((m<0)||(n<0)||(m>7)||(n>7))
        return false;

    printf("n = %d\nm = %d\n", n+1, m+1);

    ///////////////////////////////////// exact_location ///////////////////////////////////

    Punto p1(imrx(m,n),imry(m,n));
    Punto p2(imrx(m,n+1),imry(m,n+1));
    Punto p3(imrx(m+1,n+1),imry(m+1,n+1));
    Punto p4(imrx(m+1,n),imry(m+1,n));

    Recta r1(p1,p2);
    Recta r2(p2,p3);
    Recta r3(p3,p4);
    Recta r4(p4,p1);
    Punto t1 = cruceRR(r1,r3);
    Punto t2 = cruceRR(r2,r4);
    Recta rI1(Centroide,t1);
    Recta rI2(Centroide,t2);
    Punto G3 = cruceRR(r3,rI2);
    Punto G4 = cruceRR(r4,rI1);
    double d34 = distPP(p3,p4);
    double d41 = distPP(p4,p1);
    double dg3 = distPP(p4,G3);
    double dg4 = distPP(p1,G4);

    double ColorX = ((realx(m+1,n+1)-realx(m+1,n))*(dg3/d34))+realx(m+1,n);
    double ColorY=((realy(m+1,n)-realy(m,n))*(dg4/d41))+realy(m,n);
//    Real=[ColorX,ColorY];
    printf("Real: %f, %f.\n",ColorX,ColorY);

    /////////////////////////////////// correct ////////////////////////////////////////////
    // Esta funcion corrige la posición del objeto teniendo en cuenta la curvatura de la lente
    double mycent=118.5;
    double myalt=43.5;
    Recta R1(Punto(mycent,0),Punto(0,myalt));

    Recta Rp(Punto(0,0),perp(R1));
    Recta R2(Punto(realy(m+1,n),0),Punto(0,myalt));
    Recta R3(Punto(realy(m,n),0),Punto(0,myalt));
    Recta R4(Punto(ColorY,0),Punto(0,myalt));

    Punto P2 = cruceRR(R2,Rp);
    Punto P3 = cruceRR(R3,Rp);
    Punto P4 = cruceRR(R4,Rp);

    double mdist = distPP(P2,P3);
    double ndist = distPP(P3,P4);
    double finalX = ColorX;
    double finalY = ((realy(m+1,n)-realy(m,n))*(ndist/mdist))+realy(m,n);
    printf("Final: %f, %f.\n",finalX,finalY);
    outx = finalX;
    outy = finalY;

    return true;
}

