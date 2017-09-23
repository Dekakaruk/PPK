set (MODULE ${PROJECT_NAME})


add_library (${MODULE} SHARED
	Node.cpp
	IFileIterator.cpp
	FS.cpp
	)

set_property(TARGET ${MODULE} PROPERTY CXX_STANDARD 11)
target_link_libraries(${MODULE} PUBLIC Boost::filesystem)



install(
	TARGETS ${MODULE}
	EXPORT ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

install(
	FILES
		Node.hpp
		Node.tpp
		NodeIterators.tpp
		NodeIterators.hpp
		StandardConverters.hpp
		FS.hpp
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}/${MODULE}
	)

