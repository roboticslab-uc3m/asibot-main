# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2010 RobotCub Consortium
# Authors: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

IF (ENABLE_RlPlugins)

find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})
include(${YARP_MODULE_PATH}/YarpPlugin.cmake)
YARP_BEGIN_PLUGIN_LIBRARY(RlPlugins)
add_subdirectory(ravebot)
add_subdirectory(cartesianbot)
add_subdirectory(kdlbot)
add_subdirectory(skymegabot)
#add_subdirectory(../../../../contrib/pendant/ikinbot ${CMAKE_CURRENT_BINARY_DIR}/ikinbot)
#add_subdirectory(../../../../contrib/pendant/ravepart ${CMAKE_CURRENT_BINARY_DIR}/ravepart)
YARP_END_PLUGIN_LIBRARY(RlPlugins)

install(TARGETS RlPlugins DESTINATION lib)

ENDIF (ENABLE_RlPlugins)

IF (ENABLE_testRlPlugins)
  yarp_add_plugin_library_executable(testRlPlugins RlPlugins)
  install(TARGETS testRlPlugins DESTINATION bin)
ENDIF (ENABLE_testRlPlugins)

