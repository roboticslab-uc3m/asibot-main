include(YCMEPHelper)

ycm_ep_helper(OpenRAVE TYPE GIT
              STYLE GITHUB
              REPOSITORY rdiankov/openrave.git
              TAG master
              CMAKE_CACHE_ARGS "-DOPENRAVE_PLUGIN_FCLRAVE:BOOL=OFF")
