# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

project(embobj NONE)
if(WITH_EMBOBJ)
    install(DIRECTORY embobj
            DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
    install(DIRECTORY robotconfig
            DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
endif()

set(icub_firmware_shared_embobj_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/embobj PARENT_SCOPE)
