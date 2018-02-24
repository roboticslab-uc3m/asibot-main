include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(YARP QUIET)

ycm_ep_helper(ROBOTICSLAB_INSTALLATION_GUIDES TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/installation-guides.git
              TAG master
              DEPENDS YARP)
