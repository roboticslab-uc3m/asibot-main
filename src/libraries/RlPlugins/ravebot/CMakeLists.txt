# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Paul Fitzpatrick (YARP examples)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

YARP_PREPARE_DEVICE(ravebot TYPE RaveBot INCLUDE RaveBot.h WRAPPER controlboardwrapper2)

IF (NOT SKIP_ravebot)

FIND_PACKAGE(YARP REQUIRED)

# Next the OpenRAVE stuff
FIND_PACKAGE(OpenRAVE REQUIRED)

find_package(Boost ${OpenRAVE_Boost_VERSION} EXACT COMPONENTS iostreams python thread system)

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

YARP_ADD_PLUGIN(ravebot RaveBot.h IDeviceImpl.cpp IPositionImpl.cpp IVelocityImpl.cpp IEncoderImpl.cpp RateThreadImpl.cpp IControlLimitsImpl.cpp IControlModeImpl.cpp SharedArea.cpp WorldRpcResponder.cpp MobileRpcResponder.cpp ExtraCallbackPort.cpp)
set_target_properties(ravebot PROPERTIES COMPILE_FLAGS "${OpenRAVE_CXX_FLAGS}")
set_target_properties(ravebot PROPERTIES LINK_FLAGS "${OpenRAVE_LINK_FLAGS}")
TARGET_LINK_LIBRARIES(ravebot ${YARP_LIBRARIES} ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} ${Boost_THREAD_LIBRARY} ${BOOSt_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${BOOST_SYSTEM_LIBRARY})
install(TARGETS ravebot DESTINATION lib) 

# Exporting dependencies for ASIBOTConfig.cmake quite manually for now... 
set(ASIBOT_INCLUDE_DIRS ${ASIBOT_INCLUDE_DIRS} ${OpenRAVE_INCLUDE_DIRS})
set(ASIBOT_LIBRARIES ${ASIBOT_LIBRARIES} RlPlugins ravebot ${OpenRAVE_CORE_LIBRARIES} ${OPENRAVE_CORE_LIBRARY} ${Boost_THREAD_LIBRARY} ${BOOSt_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${BOOST_SYSTEM_LIBRARY} CACHE INTERNAL "appended libraries") 
set(ASIBOT_LINK_DIRS ${ASIBOT_LINK_DIRS} ${OpenRAVE_LIBRARY_DIRS} ${OPENRAVE_LINK_DIRS} CACHE INTERNAL "appended link dirs")

ENDIF (NOT SKIP_ravebot)

