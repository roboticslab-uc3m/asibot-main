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

    fx_d = DEFAULT_FX_D;
    fy_d = DEFAULT_FY_D;
    cx_d = DEFAULT_CX_D;
    cy_d = DEFAULT_CY_D;
    fx_rgb = DEFAULT_FX_RGB;
    fy_rgb = DEFAULT_FY_RGB;
    cx_rgb = DEFAULT_CX_RGB;
    cy_rgb = DEFAULT_CY_RGB;

    height = DEFAULT_HEIGHT;
    pan = DEFAULT_PAN;
    tilt = DEFAULT_TILT;

    algorithm = DEFAULT_ALGORITHM;
    locate = DEFAULT_LOCATE;
    maxNumBlobs = DEFAULT_MAX_NUM_BLOBS;
    morphClosing = DEFAULT_MORPH_CLOSING;
    morphOpening = DEFAULT_MORPH_OPENING;
    outImage = DEFAULT_OUT_IMAGE;
    outFeatures.fromString(DEFAULT_OUT_FEATURES);  // it's a bottle!!
    outFeaturesFormat = DEFAULT_OUT_FEATURES_FORMAT;
    int rateMs = DEFAULT_RATE_MS;
    seeBounding = DEFAULT_SEE_BOUNDING;
    threshold = DEFAULT_THRESHOLD;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("SegmentorThread options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");

        printf("\t--fx_d (default: \"%f\")\n",fx_d);
        printf("\t--fy_d (default: \"%f\")\n",fy_d);
        printf("\t--cx_d (default: \"%f\")\n",cx_d);
        printf("\t--cy_d (default: \"%f\")\n",cy_d);
        printf("\t--fx_rgb (default: \"%f\")\n",fx_rgb);
        printf("\t--fy_rgb (default: \"%f\")\n",fy_rgb);
        printf("\t--cx_rgb (default: \"%f\")\n",cx_rgb);
        printf("\t--cy_rgb (default: \"%f\")\n",cy_rgb);

        printf("\t--pan (default: \"%f\")\n",pan);
        printf("\t--tilt (default: \"%f\")\n",tilt);
        printf("\t--height (default: \"%f\")\n",height);

        printf("\t--algorithm (default: \"%s\")\n",algorithm.c_str());
        printf("\t--locate (centroid or bottom; default: \"%s\")\n",locate.c_str());
        printf("\t--maxNumBlobs (default: \"%d\")\n",maxNumBlobs);
        printf("\t--morphClosing (percentage, 2 or 4 okay; default: \"%f\")\n",morphClosing);
        printf("\t--morphOpening (percentage, 2 or 4 okay; default: \"%f\")\n",morphOpening);
        printf("\t--outFeatures (rawX,rawY,locX,locY,locZ,locX0,locY0,locZ0,angle,area,aspectRatio,axisFirst,axisSecond,\n \
                solidity,rectangularty,hue,sat,val,hueStdDev,satStdDev,valStdDev;\n \
                default: \"(%s)\")\n",outFeatures.toString().c_str());
        printf("\t--outFeaturesFormat (0=bottled,1=minimal; default: \"%d\")\n",outFeaturesFormat);
        printf("\t--outImage (0=rgb,1=bin; default: \"%d\")\n",outImage);
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
        printf("\t--seeBounding (0=none,1=box,2=contour,3=both; default: \"%d\")\n",seeBounding);
        printf("\t--threshold (default: \"%d\")\n",threshold);
        // Do not exit: let last layer exit so we get help from the complete chain.
    }

    if (rf.check("fx_d")) fx_d = rf.find("fx_d").asDouble();
    if (rf.check("fy_d")) fy_d = rf.find("fy_d").asDouble();
    if (rf.check("cx_d")) cx_d = rf.find("cx_d").asDouble();
    if (rf.check("cy_d")) cy_d = rf.find("cy_d").asDouble();
    if (rf.check("fx_rgb")) fx_rgb = rf.find("fx_rgb").asDouble();
    if (rf.check("fy_rgb")) fy_rgb = rf.find("fy_rgb").asDouble();
    if (rf.check("cx_rgb")) cx_rgb = rf.find("cx_rgb").asDouble();
    if (rf.check("cy_rgb")) cy_rgb = rf.find("cy_rgb").asDouble();
    if (rf.check("pan")) pan = rf.find("pan").asDouble();
    if (rf.check("tilt")) tilt = rf.find("tilt").asDouble();
    if (rf.check("height")) height = rf.find("height").asDouble();
    if (rf.check("algorithm")) algorithm = rf.find("algorithm").asString();
    if (rf.check("locate")) locate = rf.find("locate").asString();
    if (rf.check("maxNumBlobs")) maxNumBlobs = rf.find("maxNumBlobs").asInt();
    if (rf.check("morphClosing")) morphClosing = rf.find("morphClosing").asDouble();
    if (rf.check("morphOpening")) morphOpening = rf.find("morphOpening").asDouble();
    if (rf.check("outFeaturesFormat")) outFeaturesFormat = rf.find("outFeaturesFormat").asInt();

    printf("SegmentorThread using fx_d: %f, fy_d: %f, cx_d: %f, cy_d: %f.\n",
        fx_d,fy_d,cx_d,cy_d);
    printf("SegmentorThread using fx_rgb: %f, fy_rgb: %f, cx_rgb: %f, cy_rgb: %f.\n",
        fx_rgb,fy_rgb,cx_rgb,cy_rgb);
    printf("SegmentorThread using pan: %f, tilt: %f, height: %f.\n",
        pan,tilt,height);
    printf("SegmentorThread using algorithm: %s, locate: %s.\n",
        algorithm.c_str(),locate.c_str());
    printf("SegmentorThread using maxNumBlobs: %d, morphClosing: %.2f, outFeaturesFormat: %d.\n",
        maxNumBlobs,morphClosing,outFeaturesFormat);

    if (rf.check("outFeatures")) {
        outFeatures = *(rf.find("outFeatures").asList());  // simple overrride
    }   
    printf("SegmentorThread using outFeatures: (%s).\n", outFeatures.toString().c_str());

    if (rf.check("outImage")) outImage = rf.find("outImage").asInt();
    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();
    if (rf.check("threshold")) threshold = rf.find("threshold").asInt();
    if (rf.check("seeBounding")) seeBounding = rf.find("seeBounding").asInt();
    printf("SegmentorThread using outImage: %d, rateMs: %d, seeBounding: %d, threshold: %d.\n",
        outImage, rateMs, seeBounding, threshold);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    yarp::sig::Matrix H_0_c = rotZ(-90+pan);
    H_0_c.resize(4,4);
    H_0_c(0,3)=0;
    H_0_c(1,3)=0;
    H_0_c(2,3)=0;
    H_0_c(3,0)=0;
    H_0_c(3,1)=0;
    H_0_c(3,2)=0;
    H_0_c(3,3)=1;
    printf("*** H_0_c *** \n(%s)\n\n", H_0_c.toString().c_str());

    yarp::sig::Matrix H_c_k = rotX(-90.0+tilt);
    H_c_k.resize(4,4);
    //
    H_c_k(0,3)=0;
    H_c_k(1,3)=0;
    H_c_k(2,3)=0;
    H_c_k(3,0)=0;
    H_c_k(3,1)=0;
    H_c_k(3,2)=0;
    //
    H_c_k(2,3)=height;
    H_c_k(3,3)=1;
    printf("*** H_c_k *** \n(%s)\n\n", H_c_k.toString().c_str());

    H_0_k = H_0_c * H_c_k;

    if(cropSelector != 0) {
        processor.reset();
        inCropSelectorPort->setReader(processor);
    }

    this->setRate(rateMs);
    this->start();

}

/************************************************************************/
void SegmentorThread::run() {
    // printf("[SegmentorThread] run()\n");

    /*ImageOf<PixelRgb> *inYarpImg = pInImg->read(false);
    ImageOf<PixelFloat> *depth = pInDepth->read(false);
    if (inYarpImg==NULL) {
        //printf("No img yet...\n");
        return;
    };
    if (depth==NULL) {
        //printf("No depth yet...\n");
        return;
    };*/

    ImageOf<PixelRgb> inYarpImg = kinect->getImageMap();
    if (inYarpImg.height()<10) {
        //printf("No img yet...\n");
        return;
    };
    ImageOf<PixelInt> depth = kinect->getDepthMap();
    if (depth.height()<10) {
        //printf("No depth yet...\n");
        return;
    };

    // {yarp ImageOf Rgb -> openCv Mat Bgr}
    IplImage *inIplImage = cvCreateImage(cvSize(inYarpImg.width(), inYarpImg.height()),
                                         IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)inYarpImg.getIplImage(), inIplImage, CV_RGB2BGR);
    Mat inCvMat(inIplImage);

    PixelRgb green(0,255,0);
    // publish the original yarp img if crop selector invoked.
    if(cropSelector != 0) {
        //printf("1 x: %d, y: %d, w: %d, h: %d.\n",processor.x,processor.y,processor.w,processor.h);
        if( (processor.w!=0)&&(processor.h!=0)) {
            travisCrop(processor.x,processor.y,processor.w,processor.h,inCvMat);
            addRectangleOutline(inYarpImg,green,processor.x+processor.w/2.0,processor.y+processor.h/2.0,processor.w/2.0,processor.h/2.0);
        }
        outCropSelectorImg->prepare() = inYarpImg;
        outCropSelectorImg->write();
    }

    // Because Travis stuff goes with [openCv Mat Bgr] for now
    //Travis travis;  // ::Travis(quiet=true, overwrite=true);
    Travis travis(false,true);  // ::Travis(quiet=true, overwrite=true);
    travis.setCvMat(inCvMat);
    if(algorithm=="hue") travis.binarize("hue", threshold-5,threshold+5);
    else if(algorithm=="canny") travis.binarize("canny");
    else travis.binarize(algorithm.c_str(), threshold);
    travis.morphOpening( inYarpImg.width() * morphOpening / 100.0 );  // percent
    travis.morphClosing( inYarpImg.width() * morphClosing / 100.0 );  // percent
    //travis.morphOpening( morphOpening );
    //travis.morphClosing( morphClosing );
    travis.blobize(maxNumBlobs);
    vector<cv::Point> blobsXY;
    travis.getBlobsXY(blobsXY);
    vector<double> blobsAngle,blobsArea,blobsAspectRatio,blobsAxisFirst,blobsAxisSecond;
    vector<double> blobsRectangularity,blobsSolidity;
    vector<double> blobsHue,blobsSat,blobsVal,blobsHueStdDev,blobsSatStdDev,blobsValStdDev;
    travis.getBlobsArea(blobsArea);
    travis.getBlobsSolidity(blobsSolidity);
    travis.getBlobsHSV(blobsHue,blobsSat,blobsVal,blobsHueStdDev,blobsSatStdDev,blobsValStdDev);
    bool ok = travis.getBlobsAngle(0,blobsAngle);  // method: 0=box, 1=ellipse; note check for return as 1 can break
    if (!ok) {
        fprintf(stderr,"[warning] SegmentorThread: getBlobsAngle failed.\n");
        return;
    }
    travis.getBlobsAspectRatio(blobsAspectRatio,blobsAxisFirst,blobsAxisSecond);  // must be called after getBlobsAngle!!!!
    travis.getBlobsRectangularity(blobsRectangularity);  // must be called after getBlobsAngle!!!!
    Mat outCvMat = travis.getCvMat(outImage,seeBounding);
    travis.release();
    // { openCv Mat Bgr -> yarp ImageOf Rgb}
    IplImage outIplImage = outCvMat;
    cvCvtColor(&outIplImage,&outIplImage, CV_BGR2RGB);
    char sequence[] = "RGB";
    strcpy (outIplImage.channelSeq,sequence);
    ImageOf<PixelRgb> outYarpImg;
    outYarpImg.wrapIplImage(&outIplImage);
    PixelRgb blue(0,0,255);
    vector<double> mmX, mmY, mmZ;
    vector<double> mmX_0, mmY_0, mmZ_0;
    if(blobsXY.size() < 1) {
        fprintf(stderr,"[warning] SegmentorThread run(): blobsXY.size() < 1.\n");
        //return;
    }
    for( int i = 0; i < blobsXY.size(); i++) {
        addCircle(outYarpImg,blue,blobsXY[i].x,blobsXY[i].y,3);
        if (blobsXY[i].x<0) {
            fprintf(stderr,"[warning] SegmentorThread run(): blobsXY[%d].x < 0.\n",i);
            //return;
            blobsXY[i].x = 0;
        }
        if (blobsXY[i].y<0) {
            fprintf(stderr,"[warning] SegmentorThread run(): blobsXY[%d].y < 0.\n",i);
            //return;
            blobsXY[i].y = 0;
        }
        // double mmZ_tmp = depth->pixel(int(blobsXY[i].x +cx_d-cx_rgb),int(blobsXY[i].y +cy_d-cy_rgb));
        double mmZ_tmp = depth.pixel(int(blobsXY[i].x),int(blobsXY[i].y));
        double mmX_tmp = 1000.0 * ( (blobsXY[i].x - cx_d) * mmZ_tmp/1000.0 ) / fx_d;
        double mmY_tmp = 1000.0 * ( (blobsXY[i].y - cy_d) * mmZ_tmp/1000.0 ) / fy_d;
        mmZ.push_back( mmZ_tmp );
        mmX.push_back( mmX_tmp );
        mmY.push_back( mmY_tmp );

        yarp::sig::Matrix X_k_P(4,1);
        X_k_P(0,0)=mmX_tmp;
        X_k_P(1,0)=mmY_tmp;
        X_k_P(2,0)=mmZ_tmp;
        X_k_P(3,0)=1;

        yarp::sig::Matrix X_0_P = H_0_k * X_k_P;
        if (mmZ_tmp < 0.001) {
            X_0_P(0,0) = -99999999999;
            X_0_P(1,0) = -99999999999;
            X_0_P(2,0) = -99999999999;
        }
        mmX_0.push_back( X_0_P(0,0) );
        mmY_0.push_back( X_0_P(1,0) );
        mmZ_0.push_back( X_0_P(2,0) );
    }

    pOutImg->prepare() = outYarpImg;
    pOutImg->write();
    cvReleaseImage( &inIplImage );  // release the memory for the image
    outCvMat.release();  // cvReleaseImage( &outIplImage );  // release the memory for the image

    if ( ( blobsXY.size() < 1) && ( outFeaturesFormat == 1 ) ) return;

    // Take advantage we have the travis object and get features for text output
    Bottle output;
    for (int elem = 0; elem < outFeatures.size() ; elem++) {
        if ( outFeatures.get(elem).asString() == "mmX" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmX[0]);
            } else {
                Bottle locXs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locXs.addDouble(mmX[i]);
                output.addList() = locXs;
            }
        } else if ( outFeatures.get(elem).asString() == "mmY" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmY[0]);
            } else {
                Bottle locYs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locYs.addDouble(mmY[i]);
                output.addList() = locYs;
            }
        } else if ( outFeatures.get(elem).asString() == "mmZ" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmZ[0]);
            } else {
                Bottle locZs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locZs.addDouble(mmZ[i]);
                output.addList() = locZs;
            }
        } else if ( outFeatures.get(elem).asString() == "mmX0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmX_0[0]);
            } else {
                Bottle locXs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locXs.addDouble(mmX_0[i]);
                output.addList() = locXs;
            }
        } else if ( outFeatures.get(elem).asString() == "mmY0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmY_0[0]);
            } else {
                Bottle locYs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locYs.addDouble(mmY_0[i]);
                output.addList() = locYs;
            }
        } else if ( outFeatures.get(elem).asString() == "mmZ0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmZ_0[0]);
            } else {
                Bottle locZs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locZs.addDouble(mmZ_0[i]);
                output.addList() = locZs;
            }
         }else if ( outFeatures.get(elem).asString() == "mX0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmX_0[0]/1000.0);
            } else {
                Bottle locXs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locXs.addDouble(mmX_0[i]/1000.0);
                output.addList() = locXs;
            }
        } else if ( outFeatures.get(elem).asString() == "mY0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmY_0[0]/1000.0);
            } else {
                Bottle locYs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locYs.addDouble(mmY_0[i]/1000.0);
                output.addList() = locYs;
            }
        } else if ( outFeatures.get(elem).asString() == "mZ0" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(mmZ_0[0]/1000.0);
            } else {
                Bottle locZs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locZs.addDouble(mmZ_0[i]/1000.0);
                output.addList() = locZs;
            }
         } else if ( outFeatures.get(elem).asString() == "pxXpos" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsXY[0].x);
            } else {
                Bottle locXs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locXs.addDouble(blobsXY[i].x);
                output.addList() = locXs;
            }
        } else if ( outFeatures.get(elem).asString() == "pxYpos" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsXY[0].y);
            } else {
                Bottle locYs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locYs.addDouble(blobsXY[i].y);
                output.addList() = locYs;
            }
        } else if ( outFeatures.get(elem).asString() == "pxX" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsXY[0].x - cx_d);
            } else {
                Bottle locXs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locXs.addDouble(blobsXY[i].x - cx_d);
                output.addList() = locXs;
            }
        } else if ( outFeatures.get(elem).asString() == "pxY" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsXY[0].y - cy_d);
            } else {
                Bottle locYs;
                for (int i = 0; i < blobsXY.size(); i++)
                    locYs.addDouble(blobsXY[i].y - cy_d);
                output.addList() = locYs;
            }
        } else if ( outFeatures.get(elem).asString() == "angle" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsAngle[0]);
            } else {
                Bottle angles;
                for (int i = 0; i < blobsAngle.size(); i++)
                    angles.addDouble(blobsAngle[i]);
                output.addList() = angles;
            }
        } else if ( outFeatures.get(elem).asString() == "area" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsArea[0]);
            } else {
                Bottle areas;
                for (int i = 0; i < blobsArea.size(); i++)
                    areas.addDouble(blobsArea[i]);
                output.addList() = areas;
            }
        } else if ( outFeatures.get(elem).asString() == "realArea" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(  sqrt(blobsArea[0]) * mmX_0[0]/1000.0 );
            } else {
                Bottle areas;
                for (int i = 0; i < blobsArea.size(); i++)
                    areas.addDouble(  sqrt(blobsArea[i]) * mmX_0[i]/1000.0 );
                output.addList() = areas;
            }
        } else if ( outFeatures.get(elem).asString() == "aspectRatio" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsAspectRatio[0]);
            } else {
                Bottle aspectRatios;
                for (int i = 0; i < blobsAspectRatio.size(); i++)
                    aspectRatios.addDouble(blobsAspectRatio[i]);
                output.addList() = aspectRatios;
            }
        } else if ( outFeatures.get(elem).asString() == "rectangularity" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsRectangularity[0]);
            } else {
                Bottle rectangularities;
                for (int i = 0; i < blobsRectangularity.size(); i++)
                    rectangularities.addDouble(blobsRectangularity[i]);
                output.addList() = rectangularities;
            }
        } else if ( outFeatures.get(elem).asString() == "axisFirst" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsAxisFirst[0]);
            } else {
                Bottle axisFirsts;
                for (int i = 0; i < blobsAxisFirst.size(); i++)
                    axisFirsts.addDouble(blobsAxisFirst[i]);
                output.addList() = axisFirsts;
            }
        } else if ( outFeatures.get(elem).asString() == "axisSecond" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsAxisSecond[0]);
            } else {
                Bottle axisSeconds;
                for (int i = 0; i < blobsAxisSecond.size(); i++)
                    axisSeconds.addDouble(blobsAxisSecond[i]);
                output.addList() = axisSeconds;
            }
        } else if ( outFeatures.get(elem).asString() == "solidity" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsSolidity[0]);
            } else {
                Bottle solidities;
                for (int i = 0; i < blobsSolidity.size(); i++)
                    solidities.addDouble(blobsSolidity[i]);
                output.addList() = solidities;
            }
        } else if ( outFeatures.get(elem).asString() == "hue" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsHue[0]);
            } else {
                Bottle hues;
                for (int i = 0; i < blobsHue.size(); i++)
                    hues.addDouble(blobsHue[i]);
                output.addList() = hues;
            }
        } else if ( outFeatures.get(elem).asString() == "sat" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsSat[0]);
            } else {
                Bottle sats;
                for (int i = 0; i < blobsSat.size(); i++)
                    sats.addDouble(blobsSat[i]);
                output.addList() = sats;
            }
        } else if ( outFeatures.get(elem).asString() == "val" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsVal[0]);
            } else {
                Bottle vals;
                for (int i = 0; i < blobsVal.size(); i++)
                    vals.addDouble(blobsVal[i]);
                output.addList() = vals;
            }
        } else if ( outFeatures.get(elem).asString() == "hueStdDev" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsHueStdDev[0]);
            } else {
                Bottle hueStdDevs;
                for (int i = 0; i < blobsHueStdDev.size(); i++)
                    hueStdDevs.addDouble(blobsHueStdDev[i]);
                output.addList() = hueStdDevs;
            }
        } else if ( outFeatures.get(elem).asString() == "satStdDev" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsSatStdDev[0]);
            } else {
                Bottle satStdDevs;
                for (int i = 0; i < blobsSatStdDev.size(); i++)
                    satStdDevs.addDouble(blobsSatStdDev[i]);
                output.addList() = satStdDevs;
            }
        } else if ( outFeatures.get(elem).asString() == "valStdDev" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(blobsValStdDev[0]);
            } else {
                Bottle valStdDevs;
                for (int i = 0; i < blobsValStdDev.size(); i++)
                    valStdDevs.addDouble(blobsValStdDev[i]);
                output.addList() = valStdDevs;
            }
        } else if ( outFeatures.get(elem).asString() == "time" ) {
            if ( outFeaturesFormat == 1 ) {  // 0: Bottled, 1: Minimal
                output.addDouble(Time::now());
            } else {
                Bottle times;
                for (int i = 0; i < blobsArea.size(); i++)
                    times.addDouble(Time::now());
                output.addList() = times;
            }
        } else fprintf(stderr,"[SegmentorThread] warning: bogus outFeatures.\n");
    }
    pOutPort->write(output);

}

