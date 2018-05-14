if(UNIX)
	set(SB_LIB libSBReadFile.so)
else()
	set(SB_LIB SBReadFile.lib)
endif()

add_library(sb_reader SHARED IMPORTED)
set_property(TARGET sb_reader PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set_property(TARGET sb_reader PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${SB_LIB})

print_target_properties(sb_reader)
print_target_properties(boost_system)

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
		sb_reader)