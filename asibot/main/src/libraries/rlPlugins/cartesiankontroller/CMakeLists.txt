# Copyright: (C) 2010 Universidad Carlos III de Madrid
# Copyright: (C) 2009 RobotCub Consortium
# Author: Juan G. Victores
# Contrib: Ugo Pattacini (author of icubmod/cartesianController)
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

PREPARE_DEVICE(cartesiankontroller TYPE    CartesianKontroller 
                                   INCLUDE CartesianKontroller.h
                                   WRAPPER cartesiankontroller)

IF (NOT SKIP_cartesiankontroller)

   SET(CMAKE_MODULE_PATH ${YARPMODS_MODULE_PATH} ${CMAKE_MODULE_PATH})

   FIND_PACKAGE(Eigen2)

   SET(Orocos-KDL_FIND_QUIETLY ON)
   FIND_PACKAGE(orocos-kdl)

   SET(kontroller_header CartesianKontroller.h CartesianKontroller.cpp DeviceImpl.cpp RateThreadImpl.cpp)

   INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS} ${EIGEN2_INCLUDE_DIR} ${KDL_INCLUDE_DIR} ${Orocos-KDL_INCLUDE_DIR})

   ADD_LIBRARY(cartesiankontroller ${kontroller_header})

   TARGET_LINK_LIBRARIES(cartesiankontroller ${YARP_LIBRARIES} ${KDL_LIBRARIES} ${Orocos-KDL_LIBRARIES})

# Test-related
IF (enable_YARPMODS_TESTS)
   SET(CMAKE_MODULE_PATH ${YARPMODS_MODULE_PATH} ${CMAKE_MODULE_PATH})
   FIND_PACKAGE(ACE)
   INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
   ADD_EXECUTABLE(test_cartesiankontroller test_cartesiankontroller.cpp)
   TARGET_LINK_LIBRARIES(test_cartesiankontroller ${YARP_LIBRARIES} ${Orocos-KDL_LIBRARIES} ${KDL_LIBRARIES} rlPlugins)
   INSTALL(TARGETS test_cartesiankontroller DESTINATION bin)
ENDIF (enable_YARPMODS_TESTS)

# Exporting dependencies for YARPMODSConfig.cmake quite manually for now...
    set(YARPMODS_LIBRARIES ${YARPMODS_LIBRARIES} rlPlugins cartesiankontroller ${Orocos-KDL_LIBRARIES} ${KDL_LIBRARIES} CACHE INTERNAL "appended libraries") 


ENDIF (NOT SKIP_cartesiankontroller)


