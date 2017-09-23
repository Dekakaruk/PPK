include(CMakeFindDependencyMacro)

find_dependency(Boost 1.40 COMPONENTS filesystem)

include(${CMAKE_CURRENT_LIST_DIR}/Targets.cmake)
