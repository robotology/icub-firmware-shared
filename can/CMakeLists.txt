# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


project(canProtocolLib NONE)
if(WITH_CANPROTOCOLLIB)
    install(DIRECTORY canProtocolLib
            DESTINATION ${icub-firmware-shared_INCLUDE_DIR})
endif()
set_property(GLOBAL APPEND PROPERTY icub-firmware-shared_TARGETS canProtocolLib)
set_property(GLOBAL PROPERTY icub-firmware-shared_canProtocolLib_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set_property(GLOBAL PROPERTY icub-firmware-shared_canProtocolLib_INSTALL_INCLUDE_DIR ${icub-firmware-shared_INCLUDE_DIR})
