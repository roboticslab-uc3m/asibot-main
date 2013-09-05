# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Ugo; Paul Fitzpatrick (YARP device examples and tutorials)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

YARP_PREPARE_DEVICE(kdlbot TYPE KdlBot INCLUDE KdlBot.h)

IF (NOT SKIP_kdlbot)

SET(CMAKE_MODULE_PATH ${ASIBOT_MODULE_PATH} ${CMAKE_MODULE_PATH})
FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(Eigen2 REQUIRED)

#include(${CMAKE_MODULE_PATH}/FindOrocos-KDL.cmake)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${ASIBOT_INCLUDE_DIRS} ${EIGEN2_INCLUDE_DIR} ${KDL_INCLUDE_DIR} ${Orocos-KDL_INCLUDE_DIRS})  # kdl name depends on version
LINK_DIRECTORIES(${Orocos-KDL_LIBRARY_DIRS})

YARP_ADD_PLUGIN(kdlbot KdlBot.h IDeviceImpl.cpp ICartesianImpl.cpp RateThreadImpl.cpp HelperFuncs.cpp)
TARGET_LINK_LIBRARIES(kdlbot ${YARP_LIBRARIES} ${KDL_LIBRARIES} ${Orocos-KDL_LIBRARIES} TinyMath)
install(TARGETS kdlbot DESTINATION lib)

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} RlPlugins kdlbot ${KDL_LIBRARIES} ${Orocos-KDL_LIBRARIES} CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_kdlbot)

