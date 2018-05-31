
add_library(postgreslib STATIC
		src/database_io.cpp
		src/database_io.h
		src/oid.h
		src/sql_builder.cpp
		src/sql_builder.h
		src/endian_util.h
)

target_include_directories(postgreslib PUBLIC src)
target_compile_features(postgreslib PUBLIC cxx_std_17)

target_link_libraries(postgreslib
	PUBLIC
		postgres
		fmt::fmt
		nlohmann_json::nlohmann_json
		boost
		boost_thread
		boost_system
)

if(UINX)
	target_link_libraries(postgreslib PRIVATE pthread)
endif()