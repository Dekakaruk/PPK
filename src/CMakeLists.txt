set (MODULE ${PROJECT_NAME})


add_library (${MODULE} SHARED
	Node.cpp
	IFileIterator.cpp
	FS.cpp
	)

set(HEADERS
	Node.hpp
	Node.tpp
	NodeIterators.tpp
	NodeIterators.hpp
	StandardConverters.hpp
	FS.hpp
	)

target_compile_features(${MODULE} PUBLIC cxx_std_11)
target_link_libraries(${MODULE} PUBLIC Boost::filesystem)



install(
	TARGETS ${MODULE}
	EXPORT ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

install(
	FILES ${HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${MODULE}
	)



if (DOXYGEN_FOUND)
	set(DOXYGEN_EXCLUDE_SYMBOLS detail)
	set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
	
	doxygen_add_docs(doc ${PROJECT_SOURCE_DIR}/README.md ${HEADERS})
	
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html OPTIONAL DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()
