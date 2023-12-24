include(YCMEPHelper)
include(FindOrBuildPackage)

find_or_build_package(RapidJSON QUIET)

ycm_ep_helper(OpenRAVE TYPE GIT
              STYLE GITHUB
              REPOSITORY rdiankov/openrave.git
              TAG master
              # https://github.com/roboticslab-uc3m/installation-guides/issues/56
              CMAKE_CACHE_ARGS "-DOPT_VIDEORECORDING:BOOL=OFF;-DOPT_PYTHON:BOOL=OFF;-DOPT_PYTHON3:BOOL=OFF;-DOPT_FCL_COLLISION:BOOL=OFF;-DOPT_MSGPACK:BOOL=OFF;-DOPT_ENCRYPTION:BOOL=OFF"
              SHALLOW TRUE
              DEPENDS RapidJSON)
