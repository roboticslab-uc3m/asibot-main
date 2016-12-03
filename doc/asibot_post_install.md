## Simulation and Basic Control: Now what can I do?

Now that you have installed the basic ASIBOT repository, you're probably wondering what to do.

###  Initializing the communication server

Our current implementation uses <a href="http://www.yarp.it/what_is_yarp.html">YARP</a> for communication. Basic use of YARP requires the use of a centralized server. This server associates the low-level implementation of the communication ports with the names we give them. Before executing any ASIBOT module or application, please launch a yarp server:

```bash
[terminal 1] yarp server
```

### Launching the simulator

The simulator we use for ASIBOT is versatile and multi-layered. You'll learn how to change parameters and implementations of entire layers throughout these tutorials. However, for commodity, we've boundled up a basic pack of robotic joint-space simulator + inverse kinematic solver + inverse kinematic controller with default parameters you can launch with a single command: 

```bash
[terminal 2] cartesianServer
```

You should get a window similar to the one depicted on Figure 1.

<p align="center">
<img src="http://robots.uc3m.es/dox-asibot-main/ravebot-300px.png">
<div align="center">Fig. 1 - An instance of the cartesianServer module.</div>
</p>

### Changing parameters

Each layer of ASIBOT modules has parameters that can be modified three different ways. They are parsed in the following order (the second parsing overwrites the first, and the third one overwrites both):

- Default parameters defined in the class header files (*.h or *.hpp extension). You must recompile the project if you change any of these parameters.
- Configuration files (*.ini or *.xml extension). These files are downloaded from asibot-main/share and installed to asibot-main/build/share/asibot with the <i>cmake</i> command we issued. Within this folder, separate folders are maintained for modules and module layers.
- Command line (on program execution). Parameters may be modified executing a program using the following format: <i>./program --parameter new_value</i>.

Let's say, for example, our graphic card supports offscreen rendering and we want [cartesianServer](http://robots.uc3m.es/dox-asibot-main/group__cartesianServer.html) to load an enviroment that has cameras in the simulated environment. We can execute the program touching the parameters at the command line level which, as we have said, are parsed last:

```bash
[terminal 2] cartesianServer --env asibot_kitchen_cameras.env.xml
```

This specific environment contains simulated cameras that Figure 2. You can learn how to make the connections in the  <a href="http://robots.uc3m.es/dox-asibot-main/group__testRaveBot.html#testRaveBot_interfacing">testRaveBot</a> documentation.

<p align="center">
<img src="http://robots.uc3m.es/dox-asibot-main/ravebotCompare.png">
</p>

We can actually see the default parameters, and the final selected ones, with the <b>--help</b> parameter:

```bash
[terminal 2] cartesianServer --env asibot_kitchen_cameras.env.xml --help
```

If we want to affect this parameter at a more persistent level, we can change the configuration file. For this specific case, the [cartesianServer](http://robots.uc3m.es/dox-asibot-main/group__cartesianServer.html) configuration file is located at <i>repos/asibot-main/share/cartesianServer/conf/cartesianServer.ini</i>. In this file, we can see that most parameters are commented out (the <b>//</b> characters). This is a common convention to indicate these are the default parameters set in the headers (the first parsed, as explained previously). Here, we would search to subsitute the line:

```bash
// env asibot_kitchen.env.xml           /// env [xml] environment name in abs or rel
```

With a new, uncommented line:

```bash
env asibot_kitchen_cameras.env.xml           /// env [xml] environment name in abs or rel
```

## Interfacing

We can interact with this module through port commands as described in <a href="http://robots.uc3m.es/dox-asibot-main/group__testRaveBot.html#testRaveBot_interfacing">testRaveBot</a> and <a href="http://robots.uc3m.es/dox-asibot-main/group__cartesianServer.html#cartesianServer_interfacing">cartesianServer</a>, or through the different language APIs as can be seen in the different <a href="http://robots.uc3m.es/dox-asibot-main/group__asibot__examples.html">ASIBOT examples</a>.

## Even more!

Done! You are now probably interested in one of the following links:

- [Install vision](asibot_install_vision.md)
- You can now compile and install any of the [other modules](http://robots.uc3m.es/dox-asibot-main/modules.html).
- These modules are actually packed up and ready to go in our [ASIBOT applications](http://robots.uc3m.es/dox-asibot-main/group__asibot__applications.html)!
