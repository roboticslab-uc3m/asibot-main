# Copyright: (C) 2012 Universidad Carlos III de Madrid
# Author: Juan G. Victores
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

PREPARE_DEVICE(skymegabot TYPE SkymegaBot INCLUDE SkymegaBot.h WRAPPER controlboard)

IF (NOT SKIP_skymegabot)

find_package(YARP REQUIRED)

# Include any directories needed for YARP
include_directories(${YARP_INCLUDE_DIRS})

ADD_LIBRARY(skymegabot SkymegaBot.h SkymegaBot.cpp HelperFuncs.cpp)
TARGET_LINK_LIBRARIES(skymegabot ${YARP_LIBRARIES})

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} RlPlugins skymegabot CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_skymegabot)

