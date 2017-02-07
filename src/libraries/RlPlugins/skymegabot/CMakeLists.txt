# Copyright: (C) 2012 Universidad Carlos III de Madrid
# Author: Juan G. Victores
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

YARP_PREPARE_PLUGIN(skymegabot CATEGORY device TYPE SkymegaBot INCLUDE SkymegaBot.h WRAPPER controlboard)

IF (NOT SKIP_skymegabot)

find_package(YARP REQUIRED)

include_directories(${YARP_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

YARP_ADD_PLUGIN(skymegabot SkymegaBot.h SkymegaBot.cpp HelperFuncs.cpp)
TARGET_LINK_LIBRARIES(skymegabot ${YARP_LIBRARIES})
install(TARGETS skymegabot DESTINATION lib)

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} RlPlugins skymegabot CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_skymegabot)

