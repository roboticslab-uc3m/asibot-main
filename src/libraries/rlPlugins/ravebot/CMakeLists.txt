# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

PREPARE_DEVICE(ravebot TYPE RaveBot INCLUDE RaveBot.h WRAPPER controlboard)

IF (NOT SKIP_ravebot)

FIND_PACKAGE(YARP REQUIRED)

# Next the OpenRAVE stuff
FIND_PACKAGE(OpenRAVE)

find_package(Boost ${OpenRAVE_Boost_VERSION} EXACT COMPONENTS iostreams python thread)

#if( NOT OPENRAVE_FOUND )
#  message(FATAL_ERROR "could not find openrave, check for openrave-config in path")
#endif( NOT OPENRAVE_FOUND )
if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  add_definitions("-fno-strict-aliasing -Wall")
endif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
include_directories(${OpenRAVE_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
if( Boost_INCLUDE_DIRS )
  include_directories(${Boost_INCLUDE_DIRS})
endif()

link_directories(${OpenRAVE_LIBRARY_DIRS} ${OPENRAVE_LINK_DIRS}) # All caps -> older

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(ravebot RaveBot.h IDeviceImpl.cpp IPositionImpl.cpp IVelocityImpl.cpp IEncoderImpl.cpp RateThreadImpl.cpp)
TARGET_LINK_LIBRARIES(ravebot ${YARP_LIBRARIES} ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} ${Boost_THREAD_LIBRARY})

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} rlPlugins ravebot ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} ${Boost_THREAD_LIBRARY} CACHE INTERNAL "appended libraries") 
set(ASIBOT_LINK_DIRS ${ASIBOT_LINK_DIRS} ${OpenRAVE_LIBRARY_DIRS} ${OPENRAVE_LINK_DIRS} CACHE INTERNAL "appended link dirs")

ENDIF (NOT SKIP_ravebot)

