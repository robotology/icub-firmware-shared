# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(WITH_EMBOBJ)
  add_library(embobj INTERFACE)
  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS embobj)
  target_include_directories(embobj INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/embobj>
                                                      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embobj>)
  
  
  install(TARGETS embobj
          EXPORT embobj
          COMPONENT embobj
          PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/embobj)
  
  install_basic_package_files(embobj
                              VERSION ${${PROJECT_NAME}_VERSION}
                              COMPATIBILITY AnyNewerVersion
                              VARS_PREFIX embobj
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)

  install(DIRECTORY robotconfig
          DESTINATION ${icub_firmware_shared_INCLUDE_DIR})

endif()

set_property(GLOBAL PROPERTY icub_firmware_shared_embobj_INSTALL_INCLUDE_DIR ${icub_firmware_shared_INCLUDE_DIR})
