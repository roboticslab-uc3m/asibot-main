## Install OpenCV (Ubuntu 14.10)


We'll do this from source to assure the CMake file is created.

```bash
sudo apt-get install libavformat-dev libavcodec-dev libswscale-dev
wget https://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.2/OpenCV-2.4.2.tar.bz2  # superseeds: svn co https://code.ros.org/svn/opencv/trunk/opencv
tar xvf OpenCV-2.4.2.tar.bz2
cd opencv; mkdir build; cd build
cmake ..  # Make sure you see Video I/O: -- FFMPEG: YES
make -j3; sudo make install
cd ../..
```
