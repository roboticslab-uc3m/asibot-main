# A cmake file to get started with for new projects.

cmake_minimum_required(VERSION 2.6)

IF (enable_ASIBOT_MODULES)

message(STATUS "Installing webInterface.py")
#INSTALL(FILES webInterface DESTINATION bin)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/modules/webInterfacePy/webInterface.py
                                                 ${CMAKE_BINARY_DIR}/bin)

ENDIF (enable_ASIBOT_MODULES)

