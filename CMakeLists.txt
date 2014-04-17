# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 2.8.7)
project(icub-firmware-shared NONE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(icub-firmware-shared_MAJOR_VERSION 0)
set(icub-firmware-shared_MINOR_VERSION 0)
set(icub-firmware-shared_PATCH_VERSION 1)
set(icub-firmware-shared_VERSION ${icub-firmware-shared_MAJOR_VERSION}.${icub-firmware-shared_MINOR_VERSION}.${icub-firmware-shared_PATCH_VERSION})


include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FeatureSummary)


set(icub-firmware-shared_CMAKE_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/icub-firmware-shared)
set(icub-firmware-shared_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/icub-firmware-shared)

option(WITH_CANPROTOCOLLIB "Enable canProtocolLib" ON)
add_feature_info(canProtocolLib WITH_CANPROTOCOLLIB "Can Protocol Library.")

option(WITH_EMBOBJ "Enable embobj" ON)
add_feature_info(embobj WITH_EMBOBJ "EmbObj Library.")


add_subdirectory(can)
add_subdirectory(eth)




feature_summary(WHAT ENABLED_FEATURES
                     DISABLED_FEATURES)


get_property(targets GLOBAL PROPERTY icub-firmware-shared_TARGETS)
set(path_vars)
foreach(target ${targets})
    list(APPEND path_vars icub-firmware-shared_${target}_INCLUDE_DIR)
endforeach()


# icub-firmware-shared-config-version.cmake (build tree and installed)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/icub-firmware-shared-config-version.cmake
                                 VERSION ${icub-firmware-shared_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_BINARY_DIR}/icub-firmware-shared-config-version.cmake
        DESTINATION ${icub-firmware-shared_CMAKE_DESTINATION})


# icub-firmware-shared-config.cmake (build tree)
foreach(target ${targets})
    get_property(icub-firmware-shared_${target}_INCLUDE_DIR GLOBAL PROPERTY icub-firmware-shared_${target}_BUILD_INCLUDE_DIR)
    get_filename_component(icub-firmware-shared_${target}_INCLUDE_DIR "${icub-firmware-shared_${target}_INCLUDE_DIR}" ABSOLUTE)
endforeach()
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub-firmware-shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake
                              INSTALL_DESTINATION ${CMAKE_BINARY_DIR}
                              PATH_VARS ${path_vars})

# icub-firmware-shared-config.cmake (installed)
foreach(target ${targets})
    get_property(icub-firmware-shared_${target}_INCLUDE_DIR GLOBAL PROPERTY icub-firmware-shared_${target}_INSTALL_INCLUDE_DIR)
    get_filename_component(icub-firmware-shared_${target}_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${icub-firmware-shared_${target}_INCLUDE_DIR}" ABSOLUTE)
endforeach()
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/icub-firmware-shared-config.cmake.in
                              ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake.install
                              INSTALL_DESTINATION ${icub-firmware-shared_CMAKE_DESTINATION}
                              PATH_VARS ${path_vars})

install(FILES ${CMAKE_BINARY_DIR}/icub-firmware-shared-config.cmake.install
        DESTINATION ${icub-firmware-shared_CMAKE_DESTINATION}
        RENAME icub-firmware-shared-config.cmake)


# # icub-firmware-shared-targets.cmake (build tree)
# get_property(_targets GLOBAL PROPERTY icub-firmware-shared_TARGETS)
# export(TARGETS ${_targets} ${manager_extra_libs}
#        NAMESPACE ICUB_fw::
#        FILE ${CMAKE_BINARY_DIR}/icub-firmware-shared-targets.cmake)

# # icub-firmware-shared-targets.cmake (installed)
# install(EXPORT ICUB_fw
#         NAMESPACE ICUB_fw::
#         DESTINATION ${icub-firmware-shared_CMAKE_DESTINATION}
#         FILE icub-firmware-shared-targets.cmake)

include(AddUninstallTarget)
