# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2010 RobotCub Consortium
# Authors: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})

# These subdirectories are meant to be pulled into "yarpmod" or similar
add_subdirectory(../../main/src/libraries/RlPlugins/ravebot ${CMAKE_CURRENT_BINARY_DIR}/ravebot)
add_subdirectory(../../main/src/libraries/RlPlugins/cartesianbot ${CMAKE_CURRENT_BINARY_DIR}/cartesianbot)

