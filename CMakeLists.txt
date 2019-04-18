# Copyright: Universidad Carlos III de Madrid (C) 2010-2015;
#            RobotCub Consortium (C) 2009
# Authors: Juan G. Victores
# Contrib: Lorenzo Natale (author of iCub/main/CMakeLists.txt)
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ASIBOT_MAIN LANGUAGES NONE)

# Pick up our CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake
                              ${CMAKE_SOURCE_DIR}/cmake/ycm-modules)

# Uses YCM's unmerged ExternalProject.cmake module.
set(YCM_USE_CMAKE_PROPOSED ON)

# Bootstrap YCM. Keep it compatible with cmake_minimum_required().
include(YCMBootstrap)

# Superbuild phase.
include(FindOrBuildPackage)

# Pull remote packages if not found on system.
find_or_build_package(ASIBOT_CONFIGURATION_FILES)
find_or_build_package(ASIBOT_HMI)
find_or_build_package(ASIBOT_OPENRAVE_MODELS)
find_or_build_package(ROBOTICSLAB_DEVELOPER_MANUAL)
find_or_build_package(ROBOTICSLAB_INSTALLATION_GUIDES)
find_or_build_package(ROBOTICSLAB_KINEMATICS_DYNAMICS)
find_or_build_package(ROBOTICSLAB_OPENRAVE_YARP_PLUGINS)
find_or_build_package(ROBOTICSLAB_YARP_DEVICES)

# Prepare install/ directory (might be empty).
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/install)

# Install downloaded packages.
install(DIRECTORY ${CMAKE_BINARY_DIR}/install/
        DESTINATION ${CMAKE_INSTALL_PREFIX})

# Configure and create uninstall target.
include(AddUninstallTarget)
