# - Try to find PPK
# Once done this will define
#  PPK_FOUND - System has PPK
#  PPK_INCLUDE_DIRS - The PPK include directories
#  PPK_LIBRARIES - The libraries needed to use PPK


find_path(PPK_INCLUDE_DIR PPK/FS.hpp PATH_SUFFIXES PPK)
find_library(PPK_LIBRARY NAMES PPK)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PPK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(PPK  DEFAULT_MSG PPK_LIBRARY PPK_INCLUDE_DIR)

mark_as_advanced(PPK_INCLUDE_DIR PPK_LIBRARY )

set(PPK_LIBRARIES ${PPK_LIBRARY} )
set(PPK_INCLUDE_DIRS ${PPK_INCLUDE_DIR} )