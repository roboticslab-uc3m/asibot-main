
/**
 * \page install_vision_on_ubuntu Basic 2D and 3D Vision: Installation Guide (Ubuntu)

First install the depencencies:
  \li \ref install_cmake_on_ubuntu
  \li \ref install_yarp_on_ubuntu
  \li \ref install_opencv_on_ubuntu
  \li \ref install_travis_on_ubuntu

For Kinect support, install OpenNI, and then type:

\verbatim
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_KinectDeviceLocal=ON
make -j3; sudo make install; cd ../../..
\endverbatim

For Webcam support, you should also type:

\verbatim
cd $YARP_DIR
cmake .. -DDEVICE_LIBRARY_MODULES=ON -DENABLE_yarpmod_opencv_grabber=ON
make -j3; sudo make install; cd ../../..
\endverbatim

 * @section install_vision_asibot Install the Basic 2D and 3D Vision Software

Assuming you have done the steps in \ref install_asibot, type:

\verbatim
cd $ASIBOT_DIR
cmake .. -DENABLE_colorSegmentor=ON -DENABLE_colorDepth=ON -DENABLE_visionSegmentor=ON -DENABLE_visionDepth=ON
make -j3; sudo make install; cd ../../..
\endverbatim

@section install_vision_on_ubuntu_even_more Even more!

Done! You are now probably interested in one of the following links:
  \li \ref asibot_colorDepthKinect
  \li \ref asibot_colorDepthSim
  \li \ref asibot_colorSegmentorSim
  \li \ref asibot_visionDepthKinect

<hr>

This page can be edited at asibot-main/doc/install_on_ubuntu.dox.

*
*/

