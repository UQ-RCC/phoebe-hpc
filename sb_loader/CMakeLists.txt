
add_library(sb_reader SHARED IMPORTED)
set_property(TARGET sb_reader PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(UNIX)	
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSBReadFile.so)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSBReadFile.so DESTINATION lib)
else()	
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/SBReadFile.dll)
	set_property(TARGET sb_reader PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/SBReadFile.lib)
endif()

add_executable(loader
	src/sb_loader.cpp
	src/options.h
	src/options.cpp)

target_link_libraries(loader
	PRIVATE
		boost
		boost_system
		boost_filesystem
		boost_program_options
		fmt::fmt
		util
		sb_reader
)

if(UNIX)
	set_property(TARGET loader PROPERTY INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
endif()

install(TARGETS loader
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
