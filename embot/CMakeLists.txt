# Copyright: (C) 2020 iCub Tech, Istituto Italiano di Tecnologia
# Authors: Nicolo Genesio <nicolo.genesio@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(TRUE) #FIXME set a proper option
  set(LIBRARY_TARGET_NAME embot)

  set(${LIBRARY_TARGET_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_binary.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/tools/embot_tools.cpp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_utils.cpp)


  set(${LIBRARY_TARGET_NAME}_HDR  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_binary.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/tools/embot_tools.h
                                  ${CMAKE_CURRENT_SOURCE_DIR}/core/embot_core_utils.h )


  add_library(${LIBRARY_TARGET_NAME} SHARED ${${LIBRARY_TARGET_NAME}_HDR} ${${LIBRARY_TARGET_NAME}_SRC})

  add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

  target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>"
                                                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/core>"
                                                              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/tools>")
  install(TARGETS ${LIBRARY_TARGET_NAME}
          EXPORT ${LIBRARY_TARGET_NAME}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME})


endif()
