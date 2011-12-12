# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP device examples and tutorials)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

PREPARE_DEVICE(printbot TYPE PrintBot INCLUDE PrintBot.h WRAPPER controlboard)

IF (NOT SKIP_printbot)

FIND_PACKAGE(YARP REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(printbot PrintBot.h)
TARGET_LINK_LIBRARIES(printbot ${YARP_LIBRARIES}) # Works

# Test-related
IF (enable_YARPMODS_TESTS)
    SET(CMAKE_MODULE_PATH ${YARPMODS_MODULE_PATH} ${CMAKE_MODULE_PATH})
    FIND_PACKAGE(ACE)
    INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
    add_executable(test_printbot test_printbot.cpp)
    target_link_libraries(test_printbot rlPlugins)
    INSTALL(TARGETS test_printbot DESTINATION bin)
ENDIF (enable_YARPMODS_TESTS)

# Exporting dependencies for YARPMODSConfig.cmake quite manually for now... 
set(YARPMODS_LIBRARIES ${YARPMODS_LIBRARIES} rlPlugins printbot CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_printbot)

