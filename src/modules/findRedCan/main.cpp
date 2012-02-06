// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/**
 * 
 * \defgroup findRedCan
 *
 * \image html findRedCan.png
 * 
 * The <b>findRedCan</b> module tracks a red can and streams its location in the format "lata roja [cm] [cm]" through a <a href="http://eris.liralab.it/yarpdoc/classyarp_1_1os_1_1Port.html">yarp::os::Port</a> output called <b>/redCan:o</b>. The current implementation uses blobs and OpenCV, so a cvNamedWindow with the tracking is simultaneously displayed.
 *
 * <b> Legal </b>
 *
 * Copyright: 2011 (C) Universidad Carlos III de Madrid
 *
 * Author: <a href="http://roboticslab.uc3m.es/roboticslab/persona_publ.php?id_pers=72">Juan G. Victores</a>
 *
 * Contrib: Carlos Fernández-Portal (author of the MATLAB code that contained the original algorithm)
 *
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see license/LGPL.TXT
 *
 * <b>Installation</b>
 *
 * The module is compiled when enable_ASIBOT_MODULES is activated (default). For further
 * installation steps refer to <a class="el" href="pages.html">your own system installation guidelines</a>.
 *
 * <b>Running</b> (assuming correct installation)
 *
 * yarp server &
 *
 * $ASIBOT_DIR/bin/findRedCan
 *
 * <b>Modify</b>
 *
 * This file can be edited at src/modules/findRedCan/main.cpp
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "cv.h"
#include "highgui.h"

// Main blob library include
#include "BlobResult.h"

// My own image->real space converter
#include "pxToReal.h"

#include <yarp/os/all.h>

using namespace yarp::os;

int main(int argc, char** argv) {

    const bool conX = true;

    const int rMgThres = 50.0;

    Network yarp;

    if (!Network::checkNetwork()) {
        printf("No yarp network, bye!\n");
        return(-1);
    }

    Port p_xy;
    p_xy.open("/redCan:o");
    Bottle b_xy;

/*    std::string myEnvironment("");
    char *c_yarpmodsdir;
    c_yarpmodsdir = getenv("ASIBOT_DIR");
    if (c_yarpmodsdir) {
        myEnvironment.append(c_yarpmodsdir);
        myEnvironment.append("/app/findRedCan/image.jpg");
        printf("Full path+name: %s\n",myEnvironment.c_str());
    }*/

//    CvCapture* inputSource = cvCreateFileCapture("http://root:root@163.117.201.38/mjpg/video.mjpg");

    printf("Trying to connect to IP camera. This may take up to 30 seconds, please be patient...\n");

    CvCapture* inputSource = cvCreateFileCapture("http://163.117.201.38/mjpg/video.mjpg");
    if (!inputSource) {
        printf("[error] NULL inputSource. Make sure OpenCV has been compiled with Video I/O: -- FFMPEG: YES\n");
        return -1;
    }
    if (conX) {
        cvNamedWindow("Image from camera", CV_WINDOW_AUTOSIZE);
    }

    IplImage* rgb = NULL;
    IplImage* r = NULL;
    IplImage* g = NULL;
    IplImage* rMg = NULL;
//    IplImage* bloby = NULL;
    IplImage* currentFrame = NULL;

    pxToReal ipCam;

    for (;;)
    {
        currentFrame = cvQueryFrame(inputSource);

        if (!currentFrame)
            break;


        if (!rgb)
            rgb = cvCreateImage(cvSize(currentFrame->width, currentFrame->height), IPL_DEPTH_8U, currentFrame->nChannels);


        if (currentFrame->origin == IPL_ORIGIN_TL)
            cvCopy(currentFrame, rgb, 0);

        else
            cvFlip(currentFrame, rgb, 0);

        r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        g = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        rMg = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );

        cvSplit( rgb, NULL, NULL, r, NULL );  // get green as in (const rgb, b, g r, NULL)
        cvSplit( rgb, NULL, g, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)

        cvSub(r,g,rMg,NULL);

        cvCmpS( rMg, rMgThres, rMg, CV_CMP_GE ); // binarize

        // get blobs and filter them using its area
        CBlobResult blobs;

        // find blobs in image
        blobs = CBlobResult( rMg, NULL, 0 ); // 255 for inverse (black blobs on white bg)

        printf("Showing biggest out of %d blob(s)...\n",blobs.GetNumBlobs());

        // blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 30 );
        // Better than Filter:
        CBlob biggestBlob;
        blobs.GetNthBlob( CBlobGetArea(), 0, biggestBlob );

//        bloby = cvCreateImage( cvGetSize(rgb), rgb->depth,3 );

        CBlobGetXCenter getXCenter;
        CBlobGetYCenter getYCenter;

        int myx = getXCenter( biggestBlob );
        int myy = getYCenter( biggestBlob );

        printf("Image is width: %d, height: %d.\n",rgb->width,rgb->height);
        printf("Blob centroid at x: %d, y: %d.\n",myx,myy);

//        biggestBlob.FillBlob( bloby, CV_RGB(255,0,0));
        cvCircle(rgb, cvPoint(myx,myy), 10, cvScalar(0,255,0), 1);

        if (conX) {
            cvShowImage("Image from camera", rgb);
        }

        double resX,resY;
        b_xy.clear();
        b_xy.addString("lata");
        if(ipCam.localizar(myx,myy,resX,resY)) {
            printf("Final: %f, %f.\n",resX,resY);
            b_xy.addString("roja");
            b_xy.addDouble(resX);
            b_xy.addDouble(resY);
        } else {
            b_xy.addString("no detectada");
        }

        p_xy.write(b_xy);

        if(rgb!=NULL) {
            cvReleaseImage( &rgb ); //release the memory for the image
            rgb = NULL;
        }
        if(r!=NULL) {
            cvReleaseImage( &r ); //release the memory for the image
            r = NULL;
        }
        if(g!=NULL) {
            cvReleaseImage( &g ); //release the memory for the image
            g = NULL;
        }
        if(rMg!=NULL) {
            cvReleaseImage( &rMg ); //release the memory for the image
            rMg = NULL;
        }
/*        if(bloby!=NULL) {
            cvReleaseImage( &bloby ); //release the memory for the image
            bloby = NULL;
        }
        if(currentFrame!=NULL) {
            cvReleaseImage( &currentFrame ); //release the memory for the image
            currentFrame = NULL;
        }*/

//        if (cvWaitKey(10) >= 0)
        if (cvWaitKey(10) == 'q')
            break;

    }

/*    if (conX) {
        cvNamedWindow("Image from camera", CV_WINDOW_AUTOSIZE);
        cvShowImage("Image from camera", rgb);
        cvWaitKey(0);
    }*/


    cvDestroyWindow("Image from camera"); //destroy the window
    cvReleaseImage( &rgb ); //release the memory for the image
    cvReleaseImage( &r ); //release the memory for the image
    cvReleaseImage( &g ); //release the memory for the image
    cvReleaseImage( &rMg ); //release the memory for the image
//    cvReleaseImage( &bloby ); //release the memory for the image
    cvReleaseImage( &currentFrame ); //release the memory for the image

    p_xy.close();

    return 0;
}




