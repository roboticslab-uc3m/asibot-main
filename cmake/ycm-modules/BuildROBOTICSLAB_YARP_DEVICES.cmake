include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(YARP QUIET)
find_or_build_package(COLOR_DEBUG QUIET)

ycm_ep_helper(ROBOTICSLAB_YARP_DEVICES TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/yarp-devices.git
              TAG master
              DEPENDS "YARP;COLOR_DEBUG")
