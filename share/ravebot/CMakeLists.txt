# Copyright: 2014 UC3M
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname ravebot)

file(GLOB models ${CMAKE_CURRENT_SOURCE_DIR}/models/*.xml)
file(GLOB models/ivs ${CMAKE_CURRENT_SOURCE_DIR}/models/ivs/*.iv)
file(GLOB models/wrls ${CMAKE_CURRENT_SOURCE_DIR}/models/wrls/*.wrl)

yarp_install(FILES ${models} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/models)
yarp_install(FILES ${models/ivs} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/models/ivs)
yarp_install(FILES ${models/wrls} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/models/wrls)

