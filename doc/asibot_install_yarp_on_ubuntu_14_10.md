## Install YARP (Ubuntu 14.10)

We use YARP for communications. Installing YARP on Ubuntu is quite straightforward. Note that you will be prompted for your password upon using '''sudo''' a couple of times:

```bash
sudo apt-get install build-essential libace-dev subversion git
sudo apt-get install libgsl0-dev  # Needed for creating YARP lib_math we use for kinematics
sudo apt-get install libgtkmm-2.4-dev  # Needed for YARP GUIs: yarpview, gyarpmanager
sudo apt-get install libjpeg8-dev  # Needed for mjpeg_carrier for ravebot cam on web
git clone https://github.com/robotology/yarp
cd yarp; mkdir build; cd build
cmake .. -DCREATE_LIB_MATH=ON -DCREATE_GUIS=ON -DCREATE_OPTIONAL_CARRIERS=ON -DENABLE_yarpcar_mjpeg_carrier=ON
make -j3;  sudo make install; sudo ldconfig; cd  ../..
```

For additional options use ccmake instead of cmake.
