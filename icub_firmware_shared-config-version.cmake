# This is a basic version file for the Config-mode of find_package(), modified
# to find icub_firmware_shared from the source directory.
#
# It is used by write_basic_package_version_file() as input file for configure_file()
# to create a version-file which can be installed along a config.cmake file.
#
# The created file sets PACKAGE_VERSION_EXACT if the current version string and
# the requested version string are exactly the same and it sets
# PACKAGE_VERSION_COMPATIBLE if the current version is equal to the requested version.
# The tweak version component is ignored.
# The variable CVF_VERSION must be set before calling configure_file().


file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" _contents REGEX "icub_firmware_shared_[A-Z]+_VERSION")
if(_contents)
  string(REGEX REPLACE ".*icub_firmware_shared_MAJOR_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_MAJOR_VERSION "${_contents}")
  string(REGEX REPLACE ".*icub_firmware_shared_MINOR_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_MINOR_VERSION "${_contents}")
  string(REGEX REPLACE ".*icub_firmware_shared_PATCH_VERSION[ \t]+([0-9]+).*" "\\1" icub_firmware_shared_PATCH_VERSION "${_contents}")
  set(icub_firmware_shared_VERSION ${icub_firmware_shared_MAJOR_VERSION}.${icub_firmware_shared_MINOR_VERSION}.${icub_firmware_shared_PATCH_VERSION})
else()
  set(icub_firmware_shared_VERSION icub_firmware_shared_NOTFOUND)
endif()


set(PACKAGE_VERSION "${icub_firmware_shared_VERSION}")

if("${icub_firmware_shared_VERSION}" MATCHES "^([0-9]+\\.[0-9]+\\.[0-9]+)\\.") # strip the tweak version
  set(CVF_VERSION_NO_TWEAK "${CMAKE_MATCH_1}")
else()
  set(CVF_VERSION_NO_TWEAK "${icub_firmware_shared_VERSION}")
endif()

if(PACKAGE_FIND_VERSION MATCHES "^([0-9]+\\.[0-9]+\\.[0-9]+)\\.") # strip the tweak version
  set(REQUESTED_VERSION_NO_TWEAK "${CMAKE_MATCH_1}")
else()
  set(REQUESTED_VERSION_NO_TWEAK "${PACKAGE_FIND_VERSION}")
endif()

if(REQUESTED_VERSION_NO_TWEAK STREQUAL CVF_VERSION_NO_TWEAK)
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
else()
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
endif()

if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
  set(PACKAGE_VERSION_EXACT TRUE)
endif()


# if the installed or the using project don't have CMAKE_SIZEOF_VOID_P set, ignore it:
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "" OR "" STREQUAL "")
   return()
endif()

# check that the installed version has the same 32/64bit-ness as the one which is currently searching:
if(NOT CMAKE_SIZEOF_VOID_P STREQUAL "")
  math(EXPR installedBits " * 8")
  set(PACKAGE_VERSION "${PACKAGE_VERSION} (${installedBits}bit)")
  set(PACKAGE_VERSION_UNSUITABLE TRUE)
endif()
