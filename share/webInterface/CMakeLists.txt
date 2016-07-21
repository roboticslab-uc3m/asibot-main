# Copyright: 2016 UC3M
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname webInterface)

file(GLOB conf ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.ini)
file(GLOB html ${CMAKE_CURRENT_SOURCE_DIR}/html/*.*)
file(GLOB html/fig ${CMAKE_CURRENT_SOURCE_DIR}/html/fig/*.*)

yarp_install(FILES ${conf} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname})
yarp_install(FILES ${html} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/html)
yarp_install(FILES ${html/fig} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/html/fig)
