# Copyright: 2014 UC3M
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname openrave)

file(GLOB asibot ${CMAKE_CURRENT_SOURCE_DIR}/asibot/*.xml)
file(GLOB asibot/trunk/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/trunk/*.xml)
file(GLOB asibot/trunk/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/trunk/*.wrl)
file(GLOB asibot/leftArm/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/leftArm/*.xml)
file(GLOB asibot/leftArm/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/leftArm/*.wrl)
file(GLOB asibot/rightArm/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/rightArm/*.xml)
file(GLOB asibot/rightArm/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/rightArm/*.wrl)
file(GLOB asibot/leftLeg/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/leftLeg/*.xml)
file(GLOB asibot/leftLeg/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/leftLeg/*.wrl)
file(GLOB asibot/rightLeg/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/rightLeg/*.xml)
file(GLOB asibot/rightLeg/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/rightLeg/*.wrl)
file(GLOB asibot/head/xml ${CMAKE_CURRENT_SOURCE_DIR}/asibot/head/*.xml)
file(GLOB asibot/head/wrl ${CMAKE_CURRENT_SOURCE_DIR}/asibot/head/*.wrl)

yarp_install(FILES ${asibot} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot)
yarp_install(FILES ${asibot/trunk/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/trunk)
yarp_install(FILES ${asibot/trunk/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/trunk)
yarp_install(FILES ${asibot/leftArm/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/leftArm)
yarp_install(FILES ${asibot/leftArm/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/leftArm)
yarp_install(FILES ${asibot/rightArm/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/rightArm)
yarp_install(FILES ${asibot/rightArm/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/rightArm)
yarp_install(FILES ${asibot/leftLeg/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/leftLeg)
yarp_install(FILES ${asibot/leftLeg/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/leftLeg)
yarp_install(FILES ${asibot/rightLeg/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/rightLeg)
yarp_install(FILES ${asibot/rightLeg/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/rightLeg)
yarp_install(FILES ${asibot/head/xml} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/head)
yarp_install(FILES ${asibot/head/wrl} DESTINATION ${ASIBOT_CONTEXTS_INSTALL_DIR}/${appname}/asibot/head)

