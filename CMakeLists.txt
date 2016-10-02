cmake_minimum_required (VERSION 3.1)




# Cmake setup
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})



# Project setup
project (PPK)

set (CMAKE_CXX_STANDARD 11)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

set (SOURCES
    src/Node.cpp
    src/IFileIterator.cpp
    src/FS.cpp
)

set (HEADERS
    src/Node.hpp
    src/Node.tpp
    src/NodeIterators.tpp
    src/utility.hpp
    src/NodeIterators.hpp
    src/StandardConverters.hpp
    src/IFileIterator.hpp
    src/FS.hpp
)


add_custom_target(headers SOURCES ${HEADERS}) # for the sake of IDEs
add_library (${PROJECT_NAME} SHARED ${SOURCES})


# Documentation

find_package(Doxygen)
if (DOXYGEN_FOUND)
	configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target (doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif (DOXYGEN_FOUND)


# External libraries

find_package (Boost REQUIRED system filesystem)
include_directories (${Boost_INCLUDE_DIR})
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})



# Installation

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
