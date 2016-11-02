## Install OpenCV (Ubuntu 14.10)

We'll do this from source to assure the CMake file is created.

```bash
sudo apt-get install libavformat-dev libavcodec-dev libswscale-dev
wget https://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.13/opencv-2.4.13.zip
unzip opencv-2.4.13.zip
cd opencv; mkdir build; cd build
cmake ..  # Make sure you see Video I/O: -- FFMPEG: YES
make -j3; sudo make install
cd ../..
```
