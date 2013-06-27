// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SegmentorThread.hpp"

/************************************************************************/
void SegmentorThread::setIKinectDeviceDriver(IKinectDeviceDriver * _kinect) {
    kinect = _kinect;
}

/************************************************************************/
void SegmentorThread::setOutImg(BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * _pOutImg) {
    pOutImg = _pOutImg;
}

/************************************************************************/
void SegmentorThread::setOutPort(Port * _pOutPort) {
    pOutPort = _pOutPort;
}

/************************************************************************/
void SegmentorThread::init(ResourceFinder &rf) {

    fx = DEFAULT_FX;
    fy = DEFAULT_FY;
    cx = DEFAULT_CX;
    cy = DEFAULT_CY;
    algorithm = DEFAULT_ALGORITHM;
    locate = DEFAULT_LOCATE;
    maxNumBlobs = DEFAULT_MAX_NUM_BLOBS;
    int rateMs = DEFAULT_RATE_MS;
    seeBounding = DEFAULT_SEE_BOUNDING;
    threshold = DEFAULT_THRESHOLD;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("SegmentorThread options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--fx (default: \"%f\")\n",fx);
        printf("\t--fy (default: \"%f\")\n",fy);
        printf("\t--cx (default: \"%f\")\n",cx);
        printf("\t--cy (default: \"%f\")\n",cy);
        printf("\t--algorithm (default: \"%s\")\n",algorithm.c_str());
        printf("\t--locate (centroid or bottom; default: \"%s\")\n",locate.c_str());
        printf("\t--maxNumBlobs (default: \"%d\")\n",maxNumBlobs);
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
        printf("\t--seeBounding (default: \"%d\")\n",seeBounding);
        printf("\t--threshold (default: \"%d\")\n",threshold);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (rf.check("fx")) fx = rf.find("fx").asDouble();
    if (rf.check("fy")) fy = rf.find("fy").asDouble();
    if (rf.check("cx")) cx = rf.find("cx").asDouble();
    if (rf.check("cy")) cy = rf.find("cy").asDouble();
    if (rf.check("algorithm")) algorithm = rf.find("algorithm").asString();
    if (rf.check("locate")) locate = rf.find("locate").asString();
    if (rf.check("maxNumBlobs")) maxNumBlobs = rf.find("maxNumBlobs").asInt();
    printf("SegmentorThread using fx: %f, fy: %f, cx: %f, cy: %f.\n",
        fx,fy,cx,cy);
    printf("SegmentorThread using algorithm: %s, locate: %s, maxNumBlobs: %d.\n",
        algorithm.c_str(),locate.c_str(),maxNumBlobs);

    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();
    if (rf.check("threshold")) threshold = rf.find("threshold").asInt();
    if (rf.check("seeBounding")) seeBounding = rf.find("seeBounding").asInt();
    printf("SegmentorThread using rateMs: %d, seeBounding: %d, threshold: %d.\n",
        rateMs, seeBounding, threshold);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    this->setRate(rateMs);
    this->start();

}

/************************************************************************/
void SegmentorThread::run() {
    // printf("[SegmentorThread] run()\n");

    /*ImageOf<PixelRgb> *img = pInImg->read(false);
    ImageOf<PixelFloat> *depth = pInDepth->read(false);
    if (img==NULL) {
        //printf("No img yet...\n");
        return;
    };
    if (depth==NULL) {
        //printf("No depth yet...\n");
        return;
    };*/

    ImageOf<PixelRgb> img = kinect->getImageMap();
    if (img.height()<10) {
        //printf("No img yet...\n");
        return;
    };
    ImageOf<PixelInt> depth = kinect->getDepthMap();
    if (depth.height()<10) {
        //printf("No img yet...\n");
        return;
    };

    //printf("Got img & depth!\n");

    // int code = img->getPixelCode();
    // printf("[SegmentorThread] img->getPixelCode() gets pixel code: %d\n", code);
    
    IplImage *rgb = cvCreateImage(cvSize(img.width(),  
                                             img.height()), 
                                             IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)img.getIplImage(), rgb, CV_RGB2BGR);

    Bottle container;
    //ImageOf<PixelBgr> yarpReturnImage;

    IplImage* rgbMod = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );

    if (algorithm=="redMinusGreen") {
        IplImage* r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        IplImage* g = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        cvSplit( rgb, NULL, NULL, r, NULL );  // get red as in (const rgb, b, g r, NULL)
        cvSplit( rgb, NULL, g, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)
        cvSub(r,g,rgbMod,NULL);  // subtract
        cvReleaseImage( &r ); //release the memory for the image
        cvReleaseImage( &g ); //release the memory for the image
        cvCmpS( rgbMod, threshold, rgbMod, CV_CMP_GE );  // binarize
    } else if (algorithm=="greenMinusRed") {
        IplImage* g = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        IplImage* r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        cvSplit( rgb, NULL, g, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)
        cvSplit( rgb, NULL, NULL, r, NULL );  // get red as in (const rgb, b, g r, NULL)
        cvSub(g,r,rgbMod,NULL);  // subtract
        cvReleaseImage( &g ); //release the memory for the image
        cvReleaseImage( &r ); //release the memory for the image
        cvCmpS( rgbMod, threshold, rgbMod, CV_CMP_GE );  // binarize
    } else if (algorithm=="blueMinusRed") {
        IplImage* b = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        IplImage* r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        cvSplit( rgb, b, NULL, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)
        cvSplit( rgb, NULL, NULL, r, NULL );  // get red as in (const rgb, b, g r, NULL)
        cvSub(b,r,rgbMod,NULL);  // subtract
        cvReleaseImage( &b ); //release the memory for the image
        cvReleaseImage( &r ); //release the memory for the image
        cvCmpS( rgbMod, threshold, rgbMod, CV_CMP_GE );  // binarize
    } else fprintf(stderr,"[warning] Unrecognized algorithm.\n");

    // get blobs and filter them using its area
    CBlobResult blobs;

    // find blobs in image
    blobs = CBlobResult( rgbMod, NULL, 0 ); // 255 for inverse (black blobs on white bg)

    // printf("Publish biggest out of %d blob(s)...\n",blobs.GetNumBlobs());

    int numBlobs = blobs.GetNumBlobs();
    if (numBlobs > maxNumBlobs) numBlobs = maxNumBlobs;

    for (int i=0;i<numBlobs;i++) {  // from biggest to smallest
        // blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 30 );
        // Better than Filter:
        CBlob bigBlob;
        blobs.GetNthBlob( CBlobGetArea(), i, bigBlob );

        CBlobGetXCenter getXCenter;
        double myx = getXCenter( bigBlob );
        double myy;
        if (locate == "bottom") {
            CBlobGetMaxY getYMax;
            myy = getYMax( bigBlob );
        } else {  // locate == "centroid"
            CBlobGetYCenter getYCenter;
            myy = getYCenter( bigBlob );
        }

        if(seeBounding>0){
            PixelRgb green(0,255,0);
            CvRect bb = bigBlob.GetBoundingBox();
            addRectangleOutline(img,green,bb.x+bb.width/2.0,bb.y+bb.height/2.0,bb.width/2.0,bb.height/2.0);
        }

        // cvSub( rgb, r, rgb);
        // yarpReturnImage.wrapIplImage(rgb);
        // add a blue centroid/bottom circle
        PixelRgb blue(0,0,255);
        addCircle(img,blue,myx,myy,3);

        // printf("Image is width: %d, height: %d.\n",rgb->width,rgb->height);
        // printf("Blob centroid at x: %d, y: %d.\n",myx,myy);

        //Bottle b_xy;
        //b_xy.addDouble(myx);
        //b_xy.addDouble(myy);
        //container.addList() = b_xy;

        double mmZ = depth.pixel(int(myx),int(myy));  // maybe better do a mean around area?
        //fprintf(stdout,"[CallbackPort] depth at (%d,%d) is %f.\n",int(myx),int(myy),mmZ);
        Bottle mmOut;
        double mmX = 1000.0 * (myx - (cx * mmZ/1000.0)) / fx;
        double mmY = 1000.0 * (myy - (cy * mmZ/1000.0)) / fy;
        mmOut.addDouble(mmX);
        mmOut.addDouble(mmY);
        mmOut.addDouble(mmZ);
        if(mmZ != 0) container.addList() = mmOut;

    }

    cvReleaseImage( &rgbMod ); //release the memory for the image

    pOutImg->prepare() = img;
    pOutImg->write();

    pOutPort->write(container);

    cvReleaseImage( &rgb ); //release the memory for the image

}

