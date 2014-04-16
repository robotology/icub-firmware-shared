# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 2.8.7)
project(icub-firmware-shared)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(ICUB_FIRMWARE_SHARED_MAJOR_VERSION 0)
set(ICUB_FIRMWARE_SHARED_MINOR_VERSION 0)
set(ICUB_FIRMWARE_SHARED_PATCH_VERSION 1)
set(ICUB_FIRMWARE_SHARED_VERSION ${ICUB_FIRMWARE_SHARED_MAJOR_VERSION}.${ICUB_FIRMWARE_SHARED_MINOR_VERSION}.${ICUB_FIRMWARE_SHARED_PATCH_VERSION})


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)


set(ICUB_FIRMWARE_SHARED_CMAKE_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/icub-firmware-shared)
set(ICUB_FIRMWARE_SHARED_canProtocolLib_BUILD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/can)
set(ICUB_FIRMWARE_SHARED_canProtocolLib_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/icub-firmware-shared)


add_subdirectory(can)
#add_subdirectory(eth)


# icub-firmware-shared-config-version.cmake (build tree and installed)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/icub-firmware-shared-config-version.cmake
                                 VERSION ${ICUB_FIRMWARE_SHARED_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_BINARY_DIR}/icub-firmware-shared-config-version.cmake
        DESTINATION ${ICUB_FIRMWARE_SHARED_CMAKE_DESTINATION})


# icub-firmware-shared-config.cmake (build tree)
set(ICUB_FIRMWARE_SHARED_canProtocolLib_INCLUDE_DIR ${ICUB_FIRMWARE_SHARED_canProtocolLib_BUILD_INCLUDE_DIR})
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub-firmware-shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake
                              INSTALL_DESTINATION ${CMAKE_BINARY_DIR}
                              PATH_VARS ICUB_FIRMWARE_SHARED_canProtocolLib_INCLUDE_DIR)

# icub-firmware-shared-config.cmake (installed)
set(ICUB_FIRMWARE_SHARED_canProtocolLib_INCLUDE_DIR ${ICUB_FIRMWARE_SHARED_canProtocolLib_INSTALL_INCLUDE_DIR})
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub-firmware-shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake.install
                              INSTALL_DESTINATION ${ICUB_FIRMWARE_SHARED_CMAKE_DESTINATION}
                              PATH_VARS ICUB_FIRMWARE_SHARED_canProtocolLib_INCLUDE_DIR)

install(FILES ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake.install
        DESTINATION ${ICUB_FIRMWARE_SHARED_CMAKE_DESTINATION}
        RENAME icub-firmware-shared-config.cmake)


# # icub-firmware-shared-targets.cmake (build tree)
# get_property(_targets GLOBAL PROPERTY ICUB_FIRMWARE_SHARED_TARGETS)
# export(TARGETS ${_targets} ${manager_extra_libs}
#        NAMESPACE ICUB_fw::
#        FILE ${CMAKE_BINARY_DIR}/icub-firmware-shared-targets.cmake)

# # icub-firmware-shared-targets.cmake (installed)
# install(EXPORT ICUB_fw
#         NAMESPACE ICUB_fw::
#         DESTINATION ${ICUB_FIRMWARE_SHARED_CMAKE_DESTINATION}
#         FILE icub-firmware-shared-targets.cmake)
