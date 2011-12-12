A small HOW-TO
--------------
Copy an adapted version of ExternalModules.cmake into yarp2/conf (it is actually an
adapted version of yarp2/conf/ExternalModules.cmake.template).
Make YARPMODS, _without_ the devices you want in YARP. Set YARPMODS_DIR as usual.
Make and install YARP with the YARPMODS devices you want. They should be shown (once
installed) in the yarpdev --list

