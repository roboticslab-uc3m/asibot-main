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

    threshold = DEFAULT_THRESHOLD;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("SegmentorThread options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--threshold (default: \"%d\")\n",threshold);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (rf.check("threshold")) threshold = rf.find("threshold").asInt();
    printf("SegmentorThread using threshold: %d.\n",threshold);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        exit(1);
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

    IplImage* r = NULL;
    IplImage* g = NULL;
    IplImage* rMg = NULL;

    r = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
    g = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );
    rMg = cvCreateImage( cvGetSize(rgb), rgb->depth,1 );

    cvSplit( rgb, NULL, NULL, r, NULL );  // get green as in (const rgb, b, g r, NULL)
    cvSplit( rgb, NULL, g, NULL, NULL );  // get green as in (const rgb, b, g r, NULL)

    cvSub(r,g,rMg,NULL);  // resta

    cvCmpS( rMg, threshold, rMg, CV_CMP_GE );  // binarize

    // get blobs and filter them using its area
    CBlobResult blobs;

    // find blobs in image
    blobs = CBlobResult( rMg, NULL, 0 ); // 255 for inverse (black blobs on white bg)

    // printf("Publish biggest out of %d blob(s)...\n",blobs.GetNumBlobs());

    // blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 30 );
    // Better than Filter:
    CBlob biggestBlob;
    blobs.GetNthBlob( CBlobGetArea(), 0, biggestBlob );

    CBlobGetXCenter getXCenter;
    CBlobGetYCenter getYCenter;

    int myx = getXCenter( biggestBlob );
    int myy = getYCenter( biggestBlob );

    // add a blue circle
    PixelRgb blue(0,0,255);
    addCircle(*img,blue,myx,myy,10);
    pOutImg->prepare() = *img;
    pOutImg->write();

    // printf("Image is width: %d, height: %d.\n",rgb->width,rgb->height);
    // printf("Blob centroid at x: %d, y: %d.\n",myx,myy);

    double resX,resY;
    Bottle b_xy;
    b_xy.addString("lata");
    if(myPxToReal.localizar(myx,myy,resX,resY)) {
        printf("Final: %f, %f.\n",resX,resY);
        b_xy.addString("roja");
        b_xy.addDouble(resX);
        b_xy.addDouble(resY);
    } else {
        b_xy.addString("no detectada");
    }

    pOutPort->write(b_xy);

    cvReleaseImage( &rgb ); //release the memory for the image
    cvReleaseImage( &r ); //release the memory for the image
    cvReleaseImage( &g ); //release the memory for the image
    cvReleaseImage( &rMg ); //release the memory for the image

}

