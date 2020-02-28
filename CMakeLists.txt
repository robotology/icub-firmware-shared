# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 3.0.0)

project(icub_firmware_shared
        VERSION 4.0.7)

find_package(YCM 0.11.0 REQUIRED)

#set(icub_firmware_shared_MAJOR_VERSION 4)
#set(icub_firmware_shared_MINOR_VERSION 0)
#set(icub_firmware_shared_PATCH_VERSION 7)
#set(icub_firmware_shared_VERSION ${icub_firmware_shared_MAJOR_VERSION}.${icub_firmware_shared_MINOR_VERSION}.${icub_firmware_shared_PATCH_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YCM_MODULE_PATH})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FeatureSummary)
include(InstallBasicPackageFiles)

set(icub_firmware_shared_CMAKE_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/icub_firmware_shared)
set(icub_firmware_shared_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/icub-firmware-shared)

option(WITH_CANPROTOCOLLIB "Enable canProtocolLib" ON)
add_feature_info(canProtocolLib WITH_CANPROTOCOLLIB "Can Protocol Library.")

option(WITH_EMBOBJ "Enable embobj" ON)
add_feature_info(embobj WITH_EMBOBJ "EmbObj Library.")

add_subdirectory(can)
add_subdirectory(eth)

feature_summary(WHAT ENABLED_FEATURES
                     DISABLED_FEATURES)

add_library(${PROJECT_NAME} INTERFACE)


target_include_directories(${PROJECT_NAME} PUBLIC INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/can/canProtocolLib>
                                                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/eth/embobj>
                                                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/can/canProtocolLib>
                                                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/eth/embobj>)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        COMPONENT ${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install_basic_package_files(${PROJECT_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX ${PROJECT_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)

# # icub_firmware_shared-targets.cmake (build tree)
# get_property(_targets GLOBAL PROPERTY icub_firmware_shared_TARGETS)
# export(TARGETS ${_targets} ${manager_extra_libs}
#        NAMESPACE ICUB_fw::
#        FILE ${CMAKE_BINARY_DIR}/icub_firmware_shared-targets.cmake)

# # icub_firmware_shared-targets.cmake (installed)
# install(EXPORT ICUB_fw
#         NAMESPACE ICUB_fw::
#         DESTINATION ${icub_firmware_shared_CMAKE_DESTINATION}
#         FILE icub_firmware_shared-targets.cmake)

include(AddUninstallTarget)

set_property (CACHE CMAKE_INSTALL_PREFIX PROPERTY TYPE INTERNAL)
set_property (CACHE WITH_CANPROTOCOLLIB  PROPERTY TYPE INTERNAL)
set_property (CACHE WITH_EMBOBJ          PROPERTY TYPE INTERNAL)
