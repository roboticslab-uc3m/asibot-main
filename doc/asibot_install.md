## Simulation and Basic Control: Installation from Source Code

First install the dependencies:
- [Install CMake](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_cmake.md)
- [Install YARP](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_yarp.md)
- [Install OpenRAVE](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_openrave.md)

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
- [Install KDL](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_kdl.md)
- [Install CWiiD](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_cwiid.md)
- [Install GLib](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_glib.md)
- [Install OpenCV](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_opencv.md)
- [Install Travis](https://github.com/roboticslab-uc3m/installation-guides/blob/develop/install_travis.md)
