include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(YARP QUIET)
find_or_build_package(ROBOTICSLAB_KINEMATICS_DYNAMICS QUIET)

ycm_ep_helper(ASIBOT_HMI TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/asibot-hmi.git
              TAG master
              DEPENDS "YARP;ROBOTICSLAB_KINEMATICS_DYNAMICS")
