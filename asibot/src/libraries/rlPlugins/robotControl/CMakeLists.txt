
SET(LIBS robotcontrol)
SET(EXECUTABLE test_robotcontrol)
PREPARE_DEVICE(${LIBS} TYPE RobotControl
                         INCLUDE RobotControl.h
                         WRAPPER ${LIBS})

IF (NOT SKIP_robotcontrol)

   SET(CMAKE_MODULE_PATH ${RL_MODULE_PATH} ${CMAKE_MODULE_PATH})

   FIND_PACKAGE(Eigen2 REQUIRED)
   SET(Orocos-KDL_FIND_QUIETLY ON)
   FIND_PACKAGE(orocos-kdl REQUIRED)
   INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS} ${EIGEN2_INCLUDE_DIR} ${Orocos-KDL_INCLUDE_DIR})


# Search for source code.
   SET(header RobotControl.h)
   SET(source kinematic_chain.cpp CartesianKontroller2.cpp kinematics.cpp)



  ADD_LIBRARY(${LIBS} ${header} ${source})
   TARGET_LINK_LIBRARIES(${LIBS} ${YARP_LIBRARIES} ${Orocos-KDL_LIBRARIES})


   ADD_EXECUTABLE(${EXECUTABLE} ${EXECUTABLE}.cpp)

   TARGET_LINK_LIBRARIES(${EXECUTABLE} ${YARP_LIBRARIES} ${Orocos-KDL_LIBRARIES} ${KDL_LIBRARIES} rlPlugins)



ENDIF (NOT SKIP_robotcontrol)


