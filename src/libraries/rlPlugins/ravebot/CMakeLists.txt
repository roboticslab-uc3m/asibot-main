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

# Test-related
IF (enable_ASIBOT_TESTS)
    SET(CMAKE_MODULE_PATH ${ASIBOT_MODULE_PATH} ${CMAKE_MODULE_PATH})
    FIND_PACKAGE(ACE)
    INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})

#    add_executable(test_ravebot test_ravebot.cpp) # as server
#    target_link_libraries(test_ravebot ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} rlPlugins) # changed from OPENRAVE...
#   INSTALL(TARGETS test_ravebot DESTINATION bin)

    FIND_PACKAGE(Eigen2 REQUIRED)
    include(${CMAKE_MODULE_PATH}/FindOrocos-KDL.cmake)
    include_directories(${EIGEN2_INCLUDE_DIR} ${KDL_INCLUDE_DIR} ${Orocos-KDL_INCLUDE_DIRS})  # kdl name depends on version
    LINK_DIRECTORIES(${Orocos-KDL_LIBRARY_DIRS})
    add_executable(test_kdl test_kdl.cpp) # as server
    target_link_libraries(test_kdl rlPlugins ${KDL_LIBRARIES} ${Orocos-KDL_LIBRARIES})
    INSTALL(TARGETS test_kdl DESTINATION bin)
ENDIF (enable_ASIBOT_TESTS)


# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} rlPlugins ravebot ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} CACHE INTERNAL "appended libraries") 
set(ASIBOT_LINK_DIRS ${ASIBOT_LINK_DIRS} ${OpenRAVE_LIBRARY_DIRS} ${OPENRAVE_LINK_DIRS} CACHE INTERNAL "appended link dirs")

ENDIF (NOT SKIP_ravebot)
