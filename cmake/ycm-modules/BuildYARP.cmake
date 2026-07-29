include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(YCM QUIET)

ycm_ep_helper(YARP TYPE GIT
              STYLE GITHUB
              REPOSITORY robotology/yarp.git
              TAG master
              DEPENDS YCM
              CMAKE_CACHE_ARGS "-DSKIP_ACE:BOOL=ON"
              GIT_PROGRESS TRUE)
