# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


project(canProtocolLib NONE)
if(WITH_CANPROTOCOLLIB)
    install(DIRECTORY canProtocolLib
            DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
endif()
set_property(GLOBAL APPEND PROPERTY icub_firmware_shared_TARGETS canProtocolLib)

set(icub_firmware_shared_canProtocolLib_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/canProtocolLib PARENT_SCOPE)
