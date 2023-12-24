cmake_minimum_required(VERSION 3.16)

project(ASIBOT_MAIN LANGUAGES NONE)

# Pick up our CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ycm-modules)

# Find this YCM version on system, bootstrap it if not available.
set(YCM_MINIMUM_VERSION 0.16)

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

# Configure and create uninstall target.
include(AddUninstallTarget)
