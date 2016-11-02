## Simulation and Basic Control: Installation from Source Code (Ubuntu 14.10)

First install the dependencies:
- [Install CMake (Ubuntu 14.10)](asibot_install_cmake_on_ubuntu_14_10.md)
- [Install YARP (Ubuntu 14.10)](asibot_install_yarp_on_ubuntu_14_10.md)
- [Install OpenRAVE (Ubuntu 14.10)](asibot_install_openrave_on_ubuntu_14_10.md)

### Install the Simulation and Basic Control Software

Our software integrates the previous dependencies. Note that you will be prompted for your password upon using '''sudo''' a couple of times:

```bash
git clone https://github.com/roboticslab-uc3m/asibot-main.git
cd asibot-main; mkdir build; cd build
cmake .. -DENABLE_cartesianServer=ON -DENABLE_RlPlugins_cartesianbot=ON -DENABLE_RlPlugins_ravebot=ON
make -j3; sudo make install; cd ../..
```

For CMake find_package(ASIBOT REQUIRED), you may also be interested in adding the following to your bashrc or profile:
```bash
export ASIBOT_DIR=/path/to/asibot-main/build
```

For additional ASIBOT options use ccmake instead of cmake.

### Even more!

Done! You are now probably interested in one of the following links:
- [Simulation and Basic Control: Now what can I do?](asibot_post_install.md)
- For the KDL controller, you should look at the [KdlBot](http://robots.uc3m.es/dox-asibot-main/group__KdlBot.html) library documentation.

Optional dependencies:
- [Install KDL (Ubuntu 14.10)](asibot_install_kdl_on_ubuntu_14_10.md)
- [Install CWiiD (Ubuntu 14.10)](asibot_install_cwiid_on_ubuntu_14_10.md)
- [Install GLib (Ubuntu 14.10)](asibot_install_glib_on_ubuntu_14_10.md)
- [Install OpenCV (Ubuntu 14.10)](asibot_install_opencv_on_ubuntu_14_10.md)
- [Install Travis (Ubuntu 14.10)](asibot_install_travis_on_ubuntu_14_10.md)
