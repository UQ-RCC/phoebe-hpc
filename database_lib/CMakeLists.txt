
add_library(private-lib STATIC
		src/database_io.cpp
		src/database_io.h
		src/sql_builder.cpp
		src/sql_builder.h
		src/database_lib.h
)

target_link_libraries(private-lib
	PUBLIC
		postgres
		fmt::fmt
		nlohmann_json::nlohmann_json
		boost
		boost_thread
		boost_system
)

add_library(postgreslib STATIC
	src/database_lib.cpp
	src/database_lib.h
)

target_link_libraries(postgreslib PRIVATE private-lib)

if(UINX)
	target_link_libraries(postgreslib PRIVATE pthread)
endif()

target_include_directories(postgreslib PUBLIC src)

# set_target_properties(postgreslib PROPERTIES PUBLIC_HEADER src/database_lib.h)