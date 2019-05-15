include(YCMEPHelper)

ycm_ep_helper(YARP TYPE GIT
              STYLE GITHUB
              REPOSITORY robotology/yarp.git
              TAG master
              CMAKE_CACHE_ARGS "-DSKIP_ACE:BOOL=ON"
              GIT_PROGRESS TRUE)
