include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(COLOR_DEBUG QUIET)

ycm_ep_helper(ROBOTICSLAB_KINEMATICS_DYNAMICS TYPE GIT
              STYLE GITHUB
              REPOSITORY roboticslab-uc3m/kinematics-dynamics.git
              TAG master
              DEPENDS COLOR_DEBUG)
