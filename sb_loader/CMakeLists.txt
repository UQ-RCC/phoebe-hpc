# Old CMake ITK find+_pachage load
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Wrap SBReader shared library in target
add_library(sb_reader SHARED IMPORTED)
set_property(TARGET sb_reader PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(UNIX)	
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSlideBook6Reader.so)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSlideBook6Reader.so DESTINATION lib)
else()
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/SBReadFile.dll)
	set_property(TARGET sb_reader PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/SBReadFile.lib)
endif()

add_executable(loader
	src/sb_loader.cpp
	src/options.h
	src/options.cpp
	src/sb_io.h
)

target_link_libraries(loader
	PRIVATE
		boost
		boost_system
		boost_filesystem
		boost_program_options
		fmt::fmt
		util
		sb_reader
		 ${ITK_LIBRARIES}
)

if(UNIX)
	set_property(TARGET loader PROPERTY INSTALL_RPATH \$ORIGIN/../lib)
endif()

install(TARGETS loader
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
