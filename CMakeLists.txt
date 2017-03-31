# Copyright: Universidad Carlos III de Madrid (C) 2010-2015;
#            RobotCub Consortium (C) 2009 
# Authors: Juan G. Victores
# Contrib: Lorenzo Natale (author of iCub/main/CMakeLists.txt)
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.8.9)

project(ASIBOT)

### options: cpp libraries
option(ENABLE_CartesianClientLib "Choose if you want to compile CartesianClientLib" TRUE)
option(ENABLE_CartesianServerLib "Choose if you want to compile CartesianServerLib" TRUE)
option(ENABLE_ColorClientLib "Choose if you want to compile ColorClientLib" TRUE)
option(ENABLE_RavePlugins "Choose if you want to compile RavePlugins" FALSE)
option(ENABLE_RlPlugins "Choose if you want to compile RlPlugins" TRUE)
option(ENABLE_TestRaveBotLib "Choose if you want to compile RlPlugins" TRUE)
option(ENABLE_TinyMath "Choose if you want to compile TinyMath" TRUE)
option(ENABLE_TaskLib "Choose if you want to compile TaskLib" TRUE)
option(ENABLE_TrajGen "Choose if you want to compile TrajGen" TRUE)
option(ENABLE_TravisLib "Choose if you want to compile TravisLib" TRUE)

### options: cpp modules
option(ENABLE_asibotTasks "Choose if you want to compile asibotTasks" FALSE)
option(ENABLE_cartesianServer "Choose if you want to compile cartesianServer" FALSE)
option(ENABLE_colorDepth "Choose if you want to compile colorDepth" FALSE)
option(ENABLE_colorSegmentor "Choose if you want to compile colorSegmentor" FALSE)
option(ENABLE_kinectPxToReal "Choose if you want to compile kinectPxToReal" FALSE)
option(ENABLE_pointAtObjectServer "Choose if you want to compile pointAtObjectServer" FALSE)
option(ENABLE_premultH "Choose if you want to compile premultH" FALSE)
option(ENABLE_robotDraw "Choose if you want to compile robotDraw" FALSE)
option(ENABLE_taskServer "Choose if you want to compile taskServer" FALSE)
option(ENABLE_testRaveBot "Choose if you want to compile testRaveBot" FALSE)
option(ENABLE_testRlPlugins "Choose if you want to compile testRlPlugins" FALSE)
option(ENABLE_visionDepth "Choose if you want to compile visionDepth" FALSE)
option(ENABLE_visionSegmentor "Choose if you want to compile visionSegmentor" FALSE)
option(ENABLE_webInterface "Choose if you want to compile webInterface" FALSE)
option(ENABLE_wiimoteServer "Choose if you want to compile wiimoteServer" FALSE)

### options: misc
option(GENERATE_doc_html "Choose if you want to generate HTML documentation through Doxygen" FALSE)

if(MSVC)
    message(STATUS "Running on windows")
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, recommended options are: Debug or Release")
endif()
# Hide variable to MSVC users, since it is not needed
if(MSVC)
    mark_as_advanced(CMAKE_BUILD_TYPE)
endif()

######################

### this makes everything go in $ASIBOT_DIR/lib and $ASIBOT_DIR/bin
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
message(STATUS "Libraries go to ${LIBRARY_OUTPUT_PATH}")
message(STATUS "Executables go to ${EXECUTABLE_OUTPUT_PATH}")
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
##########################################

# Pick up our scripts - they are all in the cmake subdirectory
set(ASIBOT_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# let cmake use them
set(CMAKE_MODULE_PATH ${ASIBOT_MODULE_PATH})

# Bootstrap YCM
include(YCMBootstrap)

# Superbuild phase - include asibot-openrave-models
include(FindOrBuildPackage)
find_or_build_package(ASIBOT_OPENRAVE_MODELS)
#find_or_build_package(ROBOTICSLAB_OPENRAVE_YARP_PLUGINS)
#find_or_build_package(ROBOTICSLAB_KINEMATICS_DYNAMICS)

# Install downloaded packages.
if(NOT (USE_SYSTEM_ASIBOT_OPENRAVE_MODELS
        AND USE_SYSTEM_ROBOTICSLAB_OPENRAVE_YARP_PLUGINS
        AND USE_SYSTEM_ROBOTICSLAB_KINEMATICS_DYNAMICS))
    #install(DIRECTORY ${CMAKE_BINARY_DIR}/install/ DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

set(ASIBOT_INCLUDE_DIRS CACHE INTERNAL "appended header dirs" FORCE)
set(ASIBOT_LINK_DIRS CACHE INTERNAL "appended link dirs" FORCE)
set(ASIBOT_LIBRARIES CACHE INTERNAL "appended libraries" FORCE)

# add main contents
add_subdirectory(share)
add_subdirectory(doc)
#add_subdirectory(src)

if(ENABLE_tests)            # flag will exist
    add_subdirectory(test)  # once tests exist
endif()                     # :-)

# export our variables to a ASIBOTConfig.cmake creation
set(ASIBOT_LINK_DIRS ${ASIBOT_LINK_DIRS} ${LIBRARY_OUTPUT_PATH})
configure_file("${CMAKE_SOURCE_DIR}/cmake/template/ASIBOTConfig.cmake.in"
               "${CMAKE_BINARY_DIR}/ASIBOTConfig.cmake" @ONLY)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/template/ASIBOTConfigUninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/ASIBOTConfigUninstall.cmake" IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/ASIBOTConfigUninstall.cmake)

# configure setup.py
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/template/setup.py.in"
               "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

# run setup.py (install rule)
if(ENABLE_webInterface)
    install(CODE "execute_process(COMMAND python ${CMAKE_CURRENT_BINARY_DIR}/setup.py install)")
endif()
