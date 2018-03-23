include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(YARP QUIET)

ycm_ep_helper(ASIBOT_OPENRAVE_MODELS TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/asibot-openrave-models.git
              TAG master
              DEPENDS YARP)
