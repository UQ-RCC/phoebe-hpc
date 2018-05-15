

add_library(sb_reader SHARED IMPORTED)
set_property(TARGET sb_reader PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(UNIX)	
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/SBReadFile.so)
else()	
	set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/SBReadFile.dll)
	set_property(TARGET sb_reader PROPERTY IMPORTED_IMPLIB D:/SBReadFile/lib/Win/x64/Release/SBReadFile.lib)
endif()

if(TARGET sb_reader)
	message(STATUS "sb_reader is a target")
endif()
print_target_properties(sb_reader)


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