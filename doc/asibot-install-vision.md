## Install vision


First install the dependencies:
- [Install CMake](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install-cmake.md)
- [Install YARP](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install-yarp.md)
- [Install OpenCV](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install-opencv.md)
- [Install Travis](https://github.com/roboticslab-uc3m/installation-guides/blob/master/install-travis.md)
  
For Kinect support, install OpenNI, and then type:

```bash
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_KinectDeviceLocal=ON
make -j3; sudo make install; cd  # go home
```

For Webcam support, you should also type:

```bash
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_opencv_grabber=ON
make -j3; sudo make install; cd  # go home
```

## Install the Basic 2D and 3D Vision Software

Assuming you have done the steps in [ASIBOT install](asibot_install.md), type:

```bash
cd $ASIBOT_DIR
cmake .. -DENABLE_colorSegmentor=ON -DENABLE_colorDepth=ON -DENABLE_visionSegmentor=ON -DENABLE_visionDepth=ON
make -j3; sudo make install; cd  # go home
```

## Even more!

Done! You are now probably interested in one of the following links:
- [colorDepthKinect](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorDepthKinect.html)
- [colorDepthSim](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorDepthSim.html)
- [colorSegmentorSim](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorSegmentorSim.html)
- [visionDepthKinect](http://robots.uc3m.es/dox-asibot-main/group__asibot__visionDepthKinect.html)
