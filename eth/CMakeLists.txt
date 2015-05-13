# Copyright: (C) 2014 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT


project(embobj NONE)
if(WITH_EMBOBJ)
    #install(DIRECTORY embobj
    #        DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
    #install(DIRECTORY robotconfig
    #        DESTINATION ${icub_firmware_shared_INCLUDE_DIR})
endif()

set_property(GLOBAL APPEND PROPERTY icub_firmware_shared_TARGETS embobj)
set_property(GLOBAL PROPERTY icub_firmware_shared_embobj_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set_property(GLOBAL PROPERTY icub_firmware_shared_embobj_INSTALL_INCLUDE_DIR ${icub_firmware_shared_INCLUDE_DIR})
