# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 3.12)

project(icub_firmware_shared
        VERSION 1.18.100)

find_package(YCM 0.11.0 REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YCM_MODULE_PATH})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FeatureSummary)
include(InstallBasicPackageFiles)

set(icub_firmware_shared_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

### Options
option(WITH_CANPROTOCOLLIB "Enable canProtocolLib" ON)
add_feature_info(canProtocolLib WITH_CANPROTOCOLLIB "Can Protocol Library.")

option(WITH_EMBOT "Enable embot" ON)
add_feature_info(embot WITH_EMBOT "Embot Library.")

option(WITH_EMBOBJ "Enable embobj" ON)
add_feature_info(embobj WITH_EMBOBJ "EmbObj Library.")

# Shared/Dynamic or Static library?
option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" ON)

add_subdirectory(can)
add_subdirectory(embot)
add_subdirectory(eth)

feature_summary(WHAT ENABLED_FEATURES
                     DISABLED_FEATURES)

install_basic_package_files(${PROJECT_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX ${PROJECT_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)

include(AddUninstallTarget)

set_property (CACHE CMAKE_INSTALL_PREFIX PROPERTY TYPE INTERNAL)
set_property (CACHE WITH_CANPROTOCOLLIB  PROPERTY TYPE INTERNAL)
set_property (CACHE WITH_EMBOBJ          PROPERTY TYPE INTERNAL)
