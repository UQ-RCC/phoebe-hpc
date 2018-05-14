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
		util)