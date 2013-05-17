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
    int rateMs = DEFAULT_RATE_MS;
    seeBounding = DEFAULT_SEE_BOUNDING;
    threshold = DEFAULT_THRESHOLD;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("SegmentorThread options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--algorithm (default: \"%s\")\n",algorithm.c_str());
        printf("\t--locate (centroid or bottom; default: \"%s\")\n",locate.c_str());
        printf("\t--maxNumBlobs (default: \"%d\")\n",maxNumBlobs);
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
        printf("\t--seeBounding (default: \"%d\")\n",seeBounding);
        printf("\t--threshold (default: \"%d\")\n",threshold);
    }

    if (rf.check("algorithm")) algorithm = rf.find("algorithm").asString();
    if (rf.check("locate")) locate = rf.find("locate").asString();
    if (rf.check("maxNumBlobs")) maxNumBlobs = rf.find("maxNumBlobs").asInt();
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
    //printf("[SegmentorThread] run()\n");

    ImageOf<PixelRgb> *img = pInImg->read(false);
    if (img==NULL) {
        // printf("No img yet...\n");
        return;
    };
    // printf("Got img!\n");
    // int code = img->getPixelCode();
    // printf("[SegmentorThread] img->getPixelCode() gets pixel code: %d\n", code);
    
    IplImage *rgb = cvCreateImage(cvSize(img->width(),  
                                             img->height()), 
                                             IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)img->getIplImage(), rgb, CV_RGB2BGR);

    // --- ALGORITHMS START SOMEWHERE HERE ---
    //Mat imageFile = cv::imread("../app/images/pen4.jpg");
    Mat imageFile(rgb);
    Mat mask= Mat::zeros(imageFile.rows, imageFile.cols, CV_8UC1);

    //get biggest contour
    vector <Point> biggestCont = getBiggestContour(imageFile);

    //calculate mask of image with biggest contour
    calcMask(mask,biggestCont);

    //area of mask (with convexhull)
    calcArea(area,biggestCont);

    //location xy of the centre of biggest contour
    calcLocationXY(locX,locY, biggestCont);

    //object area over rect area
    calcRectangularity(rectangularity, biggestCont);

    //mass center
    calcMassCenter(massCenterlocX,massCenterlocY,biggestCont);

    //aspect ratio width/height
    calcAspectRatio(aspectRatio, axisFirst, axisSecond, biggestCont);

    //solidity. area object / area convexhull
    calcSolidity(solidity,biggestCont);

    //hsv peaks
    calcHSVPeakColor(imageFile, mask, hue_mode, hue_peak, value_mode, value_peak);

    //hsv mean and stddev
    calcHSVMeanStdDev(imageFile, mask,
                      hue_mean, hue_stddev,
                      saturation_mean, saturation_stddev,
                      value_mean, value_stddev);
   
    //*arc
    calcArcLength(arc,biggestCont);

    //*radius
    calcCircle(radius,biggestCont);

    // --- ALGORITHMS FINISH SOMEWHERE HERE ---

    cvReleaseImage( &rgb ); //release the memory for the image

/*    printf("***** FEATURES *****\n");
    printf("Area: %f\n",area);
    printf("Mass Center Location (x,y): (%f, %f)\n",massCenterlocX,massCenterlocY);
    printf("Rectangularity (areaObject/areaRectangle): %f\n",rectangularity);
    printf("Aspect Ratio (width/height): %f\n",aspectRatio);
    printf("Ellipse Axis 1: %f\n",axisFirst);
    printf("Ellipse Axis 2: %f\n",axisSecond);
    printf("Solidity (object/convexHull): %f\n",solidity);
    printf("Arc: %f\n",arc);
    printf("Radius: %f\n",radius);
    printf("Hue Peak: %f\n",hue_peak);
    printf("Hue Mean: %f\n",hue_mean);
    printf("Hue StdDev: %f\n",hue_stddev);
    printf("Saturation Peak: %f\n",saturation_peak);
    printf("Saturation Mean: %f\n",saturation_mean);
    printf("Saturation StdDev: %f\n",hue_stddev);
    printf("Value Peak: %f\n",value_peak);
    printf("Value Mean: %f\n",value_mean);
    printf("Value StdDev: %f\n",value_stddev);
    printf("Contour Location (x,y): (%f, %f)\n",locX,locY);*/

    Bottle b;
    b.addDouble(massCenterlocX);  // 1
    b.addDouble(massCenterlocY);  // 2
    b.addDouble(aspectRatio);  // 3
    b.addDouble(area);  // 4
    b.addDouble(rectangularity);  // 5
    b.addDouble(axisFirst);  // 6
    b.addDouble(axisSecond);  // 7
    b.addDouble(solidity);  // 8
    b.addDouble(arc);  // 9
    b.addDouble(radius);  // 10
    b.addDouble(hue_peak);  // 11
    b.addDouble(value_peak);  // 12
    /*b.addDouble(hue_mean);  // 13
    b.addDouble(hue_stddev);  // 14
    b.addDouble(saturation_peak);  // 15
    b.addDouble(saturation_mean);  // 16
    b.addDouble(saturation_stddev);  // 17
    b.addDouble(value_mean);  // 18
    b.addDouble(value_stddev);  // 19
    b.addDouble(locX);  // 20
    b.addDouble(locY);  // 21
    b.addDouble(value_mode);  // 22
    b.addDouble(hue_mode);  // 23
*/
    pOutPort->write(b);

    PixelRgb blue(0,0,255);
    addCircle(*img,blue,massCenterlocX,massCenterlocY,3);

    pOutImg->prepare() = *img;
    pOutImg->write();

}

/************************************************************************/

