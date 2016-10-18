# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Ugo; Paul Fitzpatrick (YARP device examples and tutorials)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

YARP_PREPARE_DEVICE(kdlbot TYPE KdlBot INCLUDE KdlBot.h)

IF (NOT SKIP_kdlbot)

SET(CMAKE_MODULE_PATH ${ASIBOT_MODULE_PATH} ${CMAKE_MODULE_PATH})
FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(orocos_kdl REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${ASIBOT_INCLUDE_DIRS} ${orocos_kdl_INCLUDE_DIRS})
LINK_DIRECTORIES(${orocos_kdl_LIBRARY_DIRS})

YARP_ADD_PLUGIN(kdlbot KdlBot.h IDeviceImpl.cpp ICartesianImpl.cpp RateThreadImpl.cpp HelperFuncs.cpp)
TARGET_LINK_LIBRARIES(kdlbot ${YARP_LIBRARIES} ${orocos_kdl_LIBRARIES} TinyMath)
install(TARGETS kdlbot DESTINATION lib)

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} RlPlugins kdlbot ${orocos_kdl_LIBRARIES} CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_kdlbot)
