# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2010 RobotCub Consortium
# Authors: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)
find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})
include(${YARP_MODULE_PATH}/YarpPlugin.cmake)
begin_plugin_library(rlPlugins)
add_subdirectory(ravebot)
add_subdirectory(cartesianbot)
add_subdirectory(../../../../contrib/pendant/ikinbot ${CMAKE_CURRENT_BINARY_DIR}/ikinbot)
add_subdirectory(../../../../contrib/pendant/kdlbot ${CMAKE_CURRENT_BINARY_DIR}/kdlbot)
add_subdirectory(../../../../contrib/pendant/cartesianbot2 ${CMAKE_CURRENT_BINARY_DIR}/cartesianbot2)
end_plugin_library(rlPlugins)

IF (ENABLE_testRlPlugins)
  add_plugin_library_executable(testRlPlugins rlPlugins)
  INSTALL(TARGETS testRlPlugins DESTINATION bin)
ENDIF (ENABLE_testRlPlugins)

