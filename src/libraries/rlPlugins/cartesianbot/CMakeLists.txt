# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP device examples and tutorials)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

PREPARE_DEVICE(cartesianbot TYPE CartesianBot INCLUDE CartesianBot.h)

IF (NOT SKIP_cartesianbot)

FIND_PACKAGE(YARP REQUIRED)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(cartesianbot CartesianBot.h ICartesianImpl.cpp)
TARGET_LINK_LIBRARIES(cartesianbot ${YARP_LIBRARIES}) # Works

# Test-related
#IF (enable_ASIBOT_TESTS)
#    SET(CMAKE_MODULE_PATH ${ASIBOT_MODULE_PATH} ${CMAKE_MODULE_PATH})
#    FIND_PACKAGE(ACE)
#    INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
#    add_executable(test_cartesianbot test_cartesianbot.cpp)
#    target_link_libraries(test_cartesianbot rlPlugins)
#    INSTALL(TARGETS test_cartesianbot DESTINATION bin)
#ENDIF (enable_ASIBOT_TESTS)

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} rlPlugins cartesianbot CACHE INTERNAL "appended libraries") 

ENDIF (NOT SKIP_cartesianbot)

