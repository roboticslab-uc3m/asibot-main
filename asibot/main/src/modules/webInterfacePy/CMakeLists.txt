# created by "yarp cmake" & Juan G Victores 2012 @ UC3M

cmake_minimum_required(VERSION 2.6)

IF (ENABLE_webInterfacePy)

message(STATUS "Installing webInterface.py")
#INSTALL(FILES webInterface DESTINATION bin)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/modules/webInterfacePy/webInterface.py
                                                 ${CMAKE_BINARY_DIR}/bin)

ENDIF (ENABLE_webInterfacePy)

