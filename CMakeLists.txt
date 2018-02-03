# Copyright: Universidad Carlos III de Madrid (C) 2010-2015;
#            RobotCub Consortium (C) 2009
# Authors: Juan G. Victores
# Contrib: Lorenzo Natale (author of iCub/main/CMakeLists.txt)
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.8.9)

project(ASIBOT_MAIN)

# Pick up our CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
                              ${CMAKE_SOURCE_DIR}/cmake/ycm-modules)

# Bootstrap YCM. Stick to v0.2.2 because of:
# https://github.com/robotology/ycm/issues/118
# ...and to keep YCM itself compatible with cmake_minimum_required().
set(YCM_TAG v0.2.2)
include(YCMBootstrap)

# Superbuild phase.
include(FindOrBuildPackage)

find_or_build_package(ASIBOT_CONFIGURATION_FILES)
find_or_build_package(ASIBOT_HMI)
find_or_build_package(ASIBOT_OPENRAVE_MODELS)
find_or_build_package(ROBOTICSLAB_DEVELOPER_MANUAL)
find_or_build_package(ROBOTICSLAB_INSTALLATION_GUIDES)
find_or_build_package(ROBOTICSLAB_KINEMATICS_DYNAMICS)
find_or_build_package(ROBOTICSLAB_OPENRAVE_YARP_PLUGINS)

# Install downloaded packages.
if(NOT (USE_SYSTEM_ASIBOT_CONFIGURATION_FILES
        AND USE_SYSTEM_ASIBOT_HMI
        AND USE_SYSTEM_ASIBOT_OPENRAVE_MODELS
        AND USE_SYSTEM_ROBOTICSLAB_DEVELOPER_MANUAL
        AND USE_SYSTEM_ROBOTICSLAB_INSTALLATION_GUIDES
        AND USE_SYSTEM_ROBOTICSLAB_KINEMATICS_DYNAMICS
        AND USE_SYSTEM_ROBOTICSLAB_OPENRAVE_YARP_PLUGINS))
    install(DIRECTORY ${CMAKE_BINARY_DIR}/install/ DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
