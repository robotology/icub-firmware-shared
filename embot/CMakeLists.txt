# Copyright: (C) 2020 iCub Tech, Istituto Italiano di Tecnologia
# Authors: Nicolo Genesio <nicolo.genesio@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(WITH_EMBOT)
  set(LIBRARY_TARGET_NAME embot)

  set(${LIBRARY_TARGET_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_binary.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_utils.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/tools/embot_tools.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_rop.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_ropframe.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_diagnostic_Node.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_diagnostic_Host.cpp
  )


  set(${LIBRARY_TARGET_NAME}_HDR  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_binary.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_utils.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/embot_tools.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_diagnostic.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_rop.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_ropframe.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_diagnostic_Node.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/prot/eth/embot_prot_eth_diagnostic_Host.h
  )


  add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_HDR} ${${LIBRARY_TARGET_NAME}_SRC})

  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

  target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/prot/eth>"
                                                              "$<INSTALL_INTERFACE:${icub_firmware_shared_INSTALL_INCLUDE_DIR}/${LIBRARY_TARGET_NAME}/core>"
                                                              "$<INSTALL_INTERFACE:${icub_firmware_shared_INSTALL_INCLUDE_DIR}/${LIBRARY_TARGET_NAME}/tools>"
                                                              "$<INSTALL_INTERFACE:${icub_firmware_shared_INSTALL_INCLUDE_DIR}/${LIBRARY_TARGET_NAME}/prot/eth>"
                             )
  set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION  ${${PROJECT_NAME}_VERSION}
                                                          PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")
  install(TARGETS ${LIBRARY_TARGET_NAME}
          EXPORT ${PROJECT_NAME}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          PUBLIC_HEADER DESTINATION ${icub_firmware_shared_INSTALL_INCLUDE_DIR}/${LIBRARY_TARGET_NAME})


endif()
