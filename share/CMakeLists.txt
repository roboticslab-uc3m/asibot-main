# Copyright: 2015 UC3M
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
# 

find_package(YARP REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
include(YarpInstallationHelpers)
yarp_configure_external_installation(asibot)

yarp_install(FILES ymanager.ini DESTINATION ${ASIBOT_DATA_INSTALL_DIR})  # Cleaner than ${ASIBOT_ROBOTS_INSTALL_DIR}/../ or similar.

### Go through single applications
add_subdirectory(ravebot)
add_subdirectory(cartesianServer)
add_subdirectory(webInterface)
