# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 2.8.9)
project(icub_firmware_shared NONE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(icub_firmware_shared_MAJOR_VERSION 4)
set(icub_firmware_shared_MINOR_VERSION 0)
set(icub_firmware_shared_PATCH_VERSION 2)
set(icub_firmware_shared_VERSION ${icub_firmware_shared_MAJOR_VERSION}.${icub_firmware_shared_MINOR_VERSION}.${icub_firmware_shared_PATCH_VERSION})


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FeatureSummary)


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


get_property(targets GLOBAL PROPERTY icub_firmware_shared_TARGETS)
set(path_vars)
foreach(target ${targets})
    list(APPEND path_vars icub_firmware_shared_${target}_INCLUDE_DIR)
endforeach()

# icub_firmware_shared-config-version.cmake (build tree and installed) (use AnyNewerVersion or ExactVersion or SameMajorVersion)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/icub_firmware_shared-config-version.cmake
                                 VERSION ${icub_firmware_shared_VERSION}
                                 COMPATIBILITY ExactVersion)

install(FILES ${CMAKE_BINARY_DIR}/icub_firmware_shared-config-version.cmake
        DESTINATION ${icub_firmware_shared_CMAKE_DESTINATION})


# icub_firmware_shared-config.cmake (build tree)
foreach(target ${targets})
    get_property(icub_firmware_shared_${target}_INCLUDE_DIR GLOBAL PROPERTY icub_firmware_shared_${target}_BUILD_INCLUDE_DIR)
    get_filename_component(icub_firmware_shared_${target}_INCLUDE_DIR "${icub_firmware_shared_${target}_INCLUDE_DIR}" ABSOLUTE)
endforeach()
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub_firmware_shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub_firmware_shared-config.cmake
                              INSTALL_DESTINATION ${CMAKE_BINARY_DIR}
                              PATH_VARS ${path_vars})

# icub_firmware_shared-config.cmake (installed)
foreach(target ${targets})
    get_property(icub_firmware_shared_${target}_INCLUDE_DIR GLOBAL PROPERTY icub_firmware_shared_${target}_INSTALL_INCLUDE_DIR)
    get_filename_component(icub_firmware_shared_${target}_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${icub_firmware_shared_${target}_INCLUDE_DIR}" ABSOLUTE)
endforeach()
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub_firmware_shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub_firmware_shared-config.cmake.install
                              INSTALL_DESTINATION ${icub_firmware_shared_CMAKE_DESTINATION}
                              PATH_VARS ${path_vars})

install(FILES ${CMAKE_BINARY_DIR}/icub_firmware_shared-config.cmake.install
        DESTINATION ${icub_firmware_shared_CMAKE_DESTINATION}
        RENAME icub_firmware_shared-config.cmake)


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
