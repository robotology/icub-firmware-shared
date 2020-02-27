# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


if(WITH_CANPROTOCOLLIB)
  set(canProtocolLib_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_analogSensorMessages.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_bootloaderMessages.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_classes.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProtocol.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_inertialSensorMessages.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_motorControlMessages.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_skinMessages.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib/iCubCanProto_types.h)

  add_library(canProtocolLib INTERFACE)
  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS canProtocolLib)
  target_include_directories(canProtocolLib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib>
                                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/canProtocolLib>)


  #target_sources(canProtocolLib PUBLIC ${canProtocolLib_HDRS})
  
  set_property(TARGET canProtocolLib PROPERTY PUBLIC_HEADER ${canProtocolLib_HDRS})
  #set_property(TARGET canProtocolLib PROPERTY VERSION ${${PROJECT_NAME}_VERSION})
  
  install(TARGETS canProtocolLib
          EXPORT canProtocolLib
          RUNTIME
            DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT canProtocolLib
          LIBRARY
            DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT canProtocolLib
          PUBLIC_HEADER
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/canProtocolLib"
            COMPONENT canProtocolLib)
endif()
set_property(GLOBAL PROPERTY icub_firmware_shared_canProtocolLib_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

