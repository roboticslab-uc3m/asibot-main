## Install OpenRAVE (Ubuntu 14.10)

We use the OpenRAVE core library for simulations. Note that you will be prompted for your password upon using '''sudo'''. To install a precompiled version of OpenRAVE, type:

```bash
sudo add-apt-repository ppa:openrave/release
sudo apt-get update
sudo apt-get install openrave
```

If nothing happens, a precompiled version of OpenRAVE might not be available on your specific Ubuntu distribution. To compile from source, type:

```bash
sudo apt-get install git-core
sudo apt-get install libboost-all-dev
sudo apt-get install libqt4-dev qt4-dev-tools libxml2-dev libode-dev
sudo apt-get install libsoqt4-dev libcoin80-dev
git clone --branch latest_stable https://github.com/rdiankov/openrave.git
cd openrave; mkdir build; cd build; cmake ..  # Use -DOPT_VIDEORECORDING=OFF if there are AV errors.
make -j3; sudo make install; cd  ../..
```
