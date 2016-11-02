## Install vision (Ubuntu 14.10)


First install the depencencies:
- [Install CMake (Ubuntu 14.10)](asibot_install_cmake_on_ubuntu_14_10.md)
- [Install YARP (Ubuntu 14.10)](asibot_install_yarp_on_ubuntu_14_10.md)
- [Install OpenCV (Ubuntu 14.10)](asibot_install_opencv_on_ubuntu_14_10.md)
- [Install Travis (Ubuntu 14.10)](asibot_install_travis_on_ubuntu_14_10.md)
  
For Kinect support, install OpenNI, and then type:

```bash
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_KinectDeviceLocal=ON
make -j3; sudo make install; cd ../../..
```

For Webcam support, you should also type:

```bash
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_opencv_grabber=ON
make -j3; sudo make install; cd ../../..
```

## Install the Basic 2D and 3D Vision Software

Assuming you have done the steps in [ASIBOT install](asibot_install_on_ubuntu_14_10.md), type:

```bash
cd $ASIBOT_DIR
cmake .. -DENABLE_colorSegmentor=ON -DENABLE_colorDepth=ON -DENABLE_visionSegmentor=ON -DENABLE_visionDepth=ON
make -j3; sudo make install; cd ../../..
```

## Even more!

Done! You are now probably interested in one of the following links:
- [colorDepthKinect](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorDepthKinect.html)
- [colorDepthSim](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorDepthSim.html)
- [colorSegmentorSim](http://robots.uc3m.es/dox-asibot-main/group__asibot__colorSegmentorSim.html)
- [visionDepthKinect](http://robots.uc3m.es/dox-asibot-main/group__asibot__visionDepthKinect.html)
