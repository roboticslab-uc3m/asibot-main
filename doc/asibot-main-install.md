## Simulation and Basic Control: Installation from Source Code

First install the dependencies:
- [Install CMake 3.5+](https://github.com/roboticslab-uc3m/installation-guides/blob/master/docs/install-cmake.md)

Additionally, this project depends on YCM to download and build external packages. Although this process is intended to run automatically during the CMake configuration phase, you may still want to install YCM and said packages by yourself. In that respect, refer to [Install YCM](https://github.com/roboticslab-uc3m/installation-guides/blob/master/docs/install-ycm.md) and to the installation guides of any package listed below:

- [asibot-configuration-files](https://github.com/roboticslab-uc3m/asibot-configuration-files)
- [asibot-hmi](https://github.com/roboticslab-uc3m/asibot-hmi)
- [asibot-openrave-models](https://github.com/roboticslab-uc3m/asibot-openrave-models)
- [developer-manual](https://github.com/roboticslab-uc3m/developer-manual)
- [installation-guides](https://github.com/roboticslab-uc3m/installation-guides)
- [kinematics-dynamics](https://github.com/roboticslab-uc3m/kinematics-dynamics)
- [openrave-yarp-plugins](https://github.com/roboticslab-uc3m/openrave-yarp-plugins)
- [yarp-devices](https://github.com/roboticslab-uc3m/yarp-devices)

### Install the Simulation and Basic Control Software

Our software integrates the previous dependencies. Note that you will be prompted for your password upon using `sudo` a couple of times:

```bash
cd  # go home
mkdir -p repos; cd repos  # make $HOME/repos if it does not exist; then, enter it
git clone https://github.com/roboticslab-uc3m/asibot-main.git  # download asibot-main software from the repository
cd asibot-main; mkdir build; cd build
cmake ..
make -j$(nproc); sudo make install; cd  # go home
```

For additional options, use `ccmake` instead of `cmake`.
