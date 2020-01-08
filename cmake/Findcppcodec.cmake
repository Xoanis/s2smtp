# Findcppcodec.cmake
#
# Finds the cppcodec library
#
# This will define the following variables
#
#    cppcodec_FOUND
#    cppcodec_INCLUDE_DIR
#
# and the following imported targets
#
#     cppcodec::cppcodec
#
# Author: Petrov Ivan - petroviv90@gmail.com
# Inspired by:
# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/ 

find_package(PkgConfig)
pkg_check_modules(PC_cppcodec QUIET cppcodec)

find_path(cppcodec_INCLUDE_DIR
    NAMES base64_rfc4648.hpp    
    PATHS ${PC_cppcodec_INCLUDE_DIRS}
    PATH_SUFFIXES cppcodec
)

set(cppcodec_VERSION ${PC_cppcodec_VERSION})

mark_as_advanced(cppcodec_FOUND cppcodec_INCLUDE_DIR cppcodec_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cppcodec
    REQUIRED_VARS cppcodec_INCLUDE_DIR
    VERSION_VAR cppcodec_VERSION
)

if(cppcodec_FOUND AND NOT TARGET cppcodec::cppcodec)
    add_library(cppcodec::cppcodec INTERFACE IMPORTED)
    set_target_properties(
        cppcodec::cppcodec 
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${cppcodec_INCLUDE_DIR}"
    )
endif()

