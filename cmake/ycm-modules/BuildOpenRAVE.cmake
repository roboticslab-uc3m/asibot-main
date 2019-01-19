include(YCMEPHelper)

ycm_ep_helper(OpenRAVE TYPE GIT
              STYLE GITHUB
              REPOSITORY rdiankov/openrave.git
              TAG master
              CMAKE_CACHE_ARGS "-DOPENRAVE_PLUGIN_FCLRAVE:BOOL=OFF;-DOPT_VIDEORECORDING:BOOL=OFF"
              # in CMake 3.6+, use: GIT_SHALLOW TRUE
              # note: does not perform --recursive (CMake 3.5, YCM 0.9)
              DOWNLOAD_COMMAND git clone --depth 1 https://github.com/rdiankov/openrave.git OpenRAVE)
