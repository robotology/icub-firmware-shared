# Copyright: (C) 2014, 2017 iCub Facility, Istituto Italiano di Tecnologia
# Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" _contents REGEX "icub_firmware_shared_[A-Z]+_VERSION")
if(_contents)
  string(REGEX REPLACE ".*icub_firmware_shared_MAJOR_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_MAJOR_VERSION "${_contents}")
  string(REGEX REPLACE ".*icub_firmware_shared_MINOR_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_MINOR_VERSION "${_contents}")
  string(REGEX REPLACE ".*icub_firmware_shared_PATCH_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_PATCH_VERSION "${_contents}")
  set(icub_firmware_shared_VERSION ${icub_firmware_shared_MAJOR_VERSION}.${icub_firmware_shared_MINOR_VERSION}.${icub_firmware_shared_PATCH_VERSION})
else()
  set(icub_firmware_shared_VERSION icub_firmware_shared_NOTFOUND)
endif()



####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was icub_firmware_shared-config.cmake.in        ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set_and_check(icub_firmware_shared_canProtocolLib_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/can")
set_and_check(icub_firmware_shared_embobj_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/eth")

set(_components canProtocolLib
                embobj)

# if(NOT TARGET YARP_fw::embObj)
#     include(${CMAKE_CURRENT_LIST_DIR}/icub_firmware_shared-targets.cmake)
# endif()

set(icub_firmware_shared_canProtocolLib_FOUND ON)
set(icub_firmware_shared_embobj_FOUND ON)

check_required_components(icub_firmware_shared)
