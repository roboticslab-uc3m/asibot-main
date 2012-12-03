// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SegmentorThread.hpp"

/************************************************************************/
void SegmentorThread::setInImg(BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * _pInImg) {
    pInImg = _pInImg;
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

    algorithm = DEFAULT_ALGORITHM;
    locate = DEFAULT_LOCATE;
    maxNumBlobs = DEFAULT_MAX_NUM_BLOBS;
    seeBounding = DEFAULT_SEE_BOUNDING;
    threshold = DEFAULT_THRESHOLD;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("SegmentorThread options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--algorithm (default: \"%s\")\n",algorithm.c_str());
        printf("\t--locate (default: \"%s\")\n",locate.c_str());
        printf("\t--maxNumBlobs (default: \"%d\")\n",maxNumBlobs);
        printf("\t--seeBounding (default: \"%d\")\n",seeBounding);
        printf("\t--threshold (default: \"%d\")\n",threshold);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (rf.check("algorithm")) algorithm = rf.find("algorithm").asString();
    if (rf.check("locate")) locate = rf.find("locate").asString();
    if (rf.check("maxNumBlobs")) maxNumBlobs = rf.find("maxNumBlobs").asInt();
    if (rf.check("threshold")) threshold = rf.find("threshold").asInt();
    printf("SegmentorThread using algorithm: %s, locate: %s, maxNumBlobs: %d, threshold: %d.\n",
        algorithm.c_str(),locate.c_str(),maxNumBlobs,threshold);

    if (rf.check("seeBounding")) seeBounding = rf.find("seeBounding").asInt();
    printf("SegmentorThread using seeBounding: %d.\n", seeBounding);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    int period = rf.check("rate",20,"ms ratethread").asInt();
    this->setRate(period);
    this->start();

}

/************************************************************************/
void SegmentorThread::run() {
    // printf("[SegmentorThread] run()\n");

    ImageOf<PixelRgb> *img = pInImg->read(false);
    if (img==NULL) {
        // printf("No img yet...\n");
        return;
    };

    // printf("Got img!\n");

    // int code = img->getPixelCode();
    // printf("[SegmentorThread] img->getPixelCode() gets pixel code: %d\n", code);
    
    ///// Get the image biggest blob /////
    IplImage *rgb = cvCreateImage(cvSize(img->width(),  
                                             img->height()), 
                                             IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)img->getIplImage(), rgb, CV_RGB2BGR);

    // --- ALGORITHM STARTS SOMEWHERE HERE ---
    
    IplImage* r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
    IplImage* g = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
    IplImage* b = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );

    cvSplit( rgb, NULL, NULL, r, NULL );  // get red as in (const rgb, b, g r, NULL)
    cvSplit( rgb, NULL, g, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)
    cvSplit( rgb, b, NULL, NULL, NULL );  // get blue as in (const rgb, b, g r, NULL)

    Bottle container;
    //ImageOf<PixelBgr> yarpReturnImage;

    if (algorithm=="redMinusGreen") {

        IplImage* rMg = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
        cvSub(r,g,rMg,NULL);  // subtract
        cvCmpS( rMg, threshold, rMg, CV_CMP_GE );  // binarize

        // get blobs and filter them using its area
        CBlobResult blobs;

        // find blobs in image
        blobs = CBlobResult( rMg, NULL, 0 ); // 255 for inverse (black blobs on white bg)

        // printf("Publish biggest out of %d blob(s)...\n",blobs.GetNumBlobs());

        int numBlobs = blobs.GetNumBlobs();
        if (numBlobs > maxNumBlobs) numBlobs = maxNumBlobs;

        for (int i=0;i<numBlobs;i++) {  // from biggest to smallest
            // blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 30 );
            // Better than Filter:
            CBlob bigBlob;
            blobs.GetNthBlob( CBlobGetArea(), i, bigBlob );

            CBlobGetXCenter getXCenter;
            int myx = getXCenter( bigBlob );
            int myy;
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
                addRectangleOutline(*img,green,bb.x+bb.width/2.0,bb.y+bb.height/2.0,bb.width/2.0,bb.height/2.0);
            }

            // cvSub( rgb, r, rgb);
            // yarpReturnImage.wrapIplImage(rgb);
            // add a blue centroid/bottom circle
            PixelRgb blue(0,0,255);
            addCircle(*img,blue,myx,myy,3);

            // printf("Image is width: %d, height: %d.\n",rgb->width,rgb->height);
            // printf("Blob centroid at x: %d, y: %d.\n",myx,myy);

            Bottle b_xy;
            b_xy.addDouble(myx);
            b_xy.addDouble(myy);
            container.addList() = b_xy;
        }

        cvReleaseImage( &rMg ); //release the memory for the image

    } else printf("[warning] Unrecognized algorithm.\n");

    pOutImg->prepare() = *img;
    pOutImg->write();

    pOutPort->write(container);

    cvReleaseImage( &rgb ); //release the memory for the image
    cvReleaseImage( &r ); //release the memory for the image
    cvReleaseImage( &g ); //release the memory for the image
    cvReleaseImage( &b ); //release the memory for the image

}

