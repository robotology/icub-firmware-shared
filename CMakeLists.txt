# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


cmake_minimum_required(VERSION 3.0.0)

project(icub_firmware_shared
        VERSION 4.0.7)

find_package(YCM 0.11.0 REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${YCM_MODULE_PATH})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FeatureSummary)
include(InstallBasicPackageFiles)

set(icub_firmware_shared_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

option(WITH_CANPROTOCOLLIB "Enable canProtocolLib" ON)
add_feature_info(canProtocolLib WITH_CANPROTOCOLLIB "Can Protocol Library.")

option(WITH_EMBOT "Enable embot" ON)
add_feature_info(embot WITH_EMBOT "Embot Library.")

option(WITH_EMBOBJ "Enable embobj" ON)
add_feature_info(embobj WITH_EMBOBJ "EmbObj Library.")

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
