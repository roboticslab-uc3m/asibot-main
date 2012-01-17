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
add_subdirectory(printbot)
add_subdirectory(ravebot)
add_subdirectory(cartesianbot)
end_plugin_library(rlPlugins)

IF (enable_ASIBOT_TESTS)
  add_plugin_library_executable(test_rlPlugins rlPlugins)
  INSTALL(TARGETS test_rlPlugins DESTINATION bin)
ENDIF (enable_ASIBOT_TESTS)

# For now, test modules are compiled inside each subdir.

