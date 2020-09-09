include(YCMEPHelper)

ycm_ep_helper(RapidJSON TYPE GIT
              STYLE GITHUB
              REPOSITORY Tencent/rapidjson.git
              TAG master
              CMAKE_CACHE_ARGS "-DRAPIDJSON_BUILD_DOC:BOOL=OFF;-DRAPIDJSON_BUILD_EXAMPLES:BOOL=OFF;-DRAPIDJSON_BUILD_TESTS:BOOL=OFF")
