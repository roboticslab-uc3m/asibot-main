# A cmake file to get started with for new projects.

cmake_minimum_required(VERSION 2.6)

IF (enable_YARPMODS_MODULES)

message(STATUS "Installing webInterface")
#INSTALL(FILES webInterface DESTINATION bin)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/modules/webInterface/webInterface.py
                                                 ${CMAKE_BINARY_DIR}/bin)

ENDIF (enable_YARPMODS_MODULES)

