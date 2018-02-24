include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(OpenRAVE QUIET)
find_or_build_package(YARP QUIET)

ycm_ep_helper(ROBOTICSLAB_OPENRAVE_YARP_PLUGINS TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/openrave-yarp-plugins.git
              TAG master
              DEPENDS "OpenRAVE;YARP")
