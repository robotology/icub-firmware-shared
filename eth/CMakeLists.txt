# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(WITH_EMBOBJ)
  set(LIBRARY_TARGET_NAME embobj)
  
  set(${LIBRARY_TARGET_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOaction.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOarray.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EoCommon.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstarray.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstvector.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOdeque.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoByte.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifo.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoWord.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOlist.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONmutex.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtask.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheCallbackManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheSystem.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheTimerManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOpacket.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOsm.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheErrorManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheLEDpulser.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheMemoryPool.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtimer.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOumlsm.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOvector.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVmutex.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtask.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheSystem.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheCallbackManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheTimerManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYmutex.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYtheSystem.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoAnalogSensors.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/FeatureInterface.extract.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoBoards.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoError.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoManagement.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoMotionControl.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoUpdaterProtocol.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/opcprot/OPCprotocolManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolAS_fun.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolAS_rom.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocol.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolEPs.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMC_fun.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMC_rom.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMN_fun.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMN_fun.new.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMN_rom.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMN_rom.new.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolSK_fun.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolSK_rom.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOagent.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOconfirmationManager.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOdeviceTransceiver.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOhostTransceiver.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnv.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvsetBRDbuilder.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvSet.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOprotocolConfigurator.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOproxy.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOreceiver.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOrop.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOropframe.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheBOARDtransceiver.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheFormer.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheInfoDispatcher.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheParser.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransceiver.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransmitter.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eODeb_eoProtoParser.c
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eOtheEthLowLevelParser.c)
  
  set(${LIBRARY_TARGET_NAME}_HDR ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/embOBJporting.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOaction.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOaction_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOarray.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOarray_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EoCommon.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstarray.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstarray_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstvector.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOconstvector_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOdeque.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOdeque_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoByte.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoByte_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifo.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifo_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoWord.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOfifoWord_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOlist.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOlist_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONmutex.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONmutex_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtask.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtask_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheCallbackManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheCallbackManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheSystem.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheSystem_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheTimerManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EONtheTimerManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOpacket.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOpacket_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOsm.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOsm_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheErrorManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheErrorManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheLEDpulser.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheLEDpulser_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheMemoryPool.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtheMemoryPool_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtimer.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOtimer_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOumlsm.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOumlsm_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOvector.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOvector_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVmutex.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVmutex_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtask.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtask_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheCallbackManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheCallbackManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheSystem.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheSystem_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheTimerManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core/EOVtheTimerManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYmutex.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYmutex_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYtheSystem.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp/EOYtheSystem_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoAnalogSensors.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoBoards.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoDiagnostics.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoError.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoManagement.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoMeasures.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoMotionControl.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoSkin.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoUpdaterProtocol.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub/EoUtilities.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/opcprot/OPCprotocolManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/opcprot/OPCprotocolManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api/EoProtocolAS.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api/EoProtocol.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api/EoProtocolMC.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api/EoProtocolMN.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api/EoProtocolSK.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/cfg/EoProtocolAS_overridden_fun.template.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/cfg/EoProtocolMC_overridden_fun.template.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/cfg/EoProtocolMN_overridden_fun.template.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/cfg/EoProtocolSK_overridden_fun.template.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolAS_rom.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolEPs.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMC_rom.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolMN_rom.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src/EoProtocolSK_rom.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOagent.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOagent_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOconfirmationManager.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOconfirmationManager_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOdeviceTransceiver.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOdeviceTransceiver_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOhostTransceiver.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOhostTransceiver_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnv.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnv_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvsetBRDbuilder.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvsetBRDbuilder_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvSet.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOnvSet_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOprotocolConfigurator.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOprotocolConfigurator_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOproxy.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOproxy_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOreceiver.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOreceiver_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOropframe.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOropframe_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOrop.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOrop_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheBOARDtransceiver.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheBOARDtransceiver_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheFormer.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheFormer_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheInfoDispatcher.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheInfoDispatcher_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheParser.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtheParser_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransceiver.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransceiver_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransmitter.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport/EOtransmitter_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eODeb_eoProtoParser.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eODeb_eoProtoParser_hid.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eOtheEthLowLevelParser.h
                                 ${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils/eOtheEthLowLevelParser_hid.h)


  add_library(${LIBRARY_TARGET_NAME} SHARED ${${LIBRARY_TARGET_NAME}_HDR} ${${LIBRARY_TARGET_NAME}_SRC})
  
  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})
  
  target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/core>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/core/exec/yarp>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/icub>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/opcprot>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/api>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/cfg>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/protocol/src>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/comm-v2/transport>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj/plus/utils>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/core/core>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/core/exec/yarp>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/icub>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/opcprot>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/protocol/api>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/protocol/cfg>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/protocol/src>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/comm-v2/transport>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj/plus/utils>")
  install(TARGETS ${LIBRARY_TARGET_NAME}
          EXPORT ${LIBRARY_TARGET_NAME}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME})
  install(DIRECTORY embobj
            DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
  install(DIRECTORY robotconfig
          DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
endif()
