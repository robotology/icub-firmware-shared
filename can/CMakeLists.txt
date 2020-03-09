# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(WITH_CANPROTOCOLLIB)
  set(LIBRARY_TARGET_NAME canProtocolLib)
  
  set(${LIBRARY_TARGET_NAME}_HDR ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_analogSensorMessages.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_bootloaderMessages.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_classes.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProtocol.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_inertialSensorMessages.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_motorControlMessages.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_skinMessages.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_types.h)
  
  add_library(${LIBRARY_TARGET_NAME} INTERFACE)
  
  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})
  
  target_include_directories(${LIBRARY_TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib>"
                                                              "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
  install(TARGETS ${LIBRARY_TARGET_NAME}
          EXPORT  ${PROJECT_NAME})
  
  install(FILES ${${LIBRARY_TARGET_NAME}_HDR}
          DESTINATION ${icub_firmware_shared_INCLUDE_DIR}/${LIBRARY_TARGET_NAME})
endif()
