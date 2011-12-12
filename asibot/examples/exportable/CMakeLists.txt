# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2010 RobotCub Consortium
# Authors: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})

# These subdirectories are meant to be pulled into yarpdev or similar
add_subdirectory(../../src/libraries/rlPlugins/printbot ${CMAKE_CURRENT_BINARY_DIR}/printbot)
add_subdirectory(../../src/libraries/rlPlugins/ravebot ${CMAKE_CURRENT_BINARY_DIR}/ravebot)
add_subdirectory(../../src/libraries/rlPlugins/cartesiankontroller ${CMAKE_CURRENT_BINARY_DIR}/cartesiankontroller)

