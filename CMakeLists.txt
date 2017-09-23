cmake_minimum_required (VERSION 3.1)

project(PPK VERSION 0.1)


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(Boost 1.40 REQUIRED COMPONENTS filesystem)
find_package(Doxygen)


if(DOXYGEN_FOUND)
	add_subdirectory(doc)
endif()

add_subdirectory(src)


set(CMAKE_CONFIG_TARGET "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")

install(
	EXPORT ${PROJECT_NAME}
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_CONFIG_TARGET}
	FILE "Targets.cmake"
	)

write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY SameMajorVersion)
install(FILES ${CONFIG_VERSION_FILE} DESTINATION ${CMAKE_CONFIG_TARGET})

install(FILES "Config.cmake" RENAME "${PROJECT_NAME}Config.cmake" DESTINATION ${CMAKE_CONFIG_TARGET})
