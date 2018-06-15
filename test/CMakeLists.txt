find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable (test src/test.cpp)

target_compile_features(test PRIVATE cxx_std_17)

target_link_libraries(test
	PRIVATE
		boost
		boost_thread
		boost_system
		boost_date_time
		boost_regex
		boost_chrono
		util
		fmt::fmt
		nlohmann_json::nlohmann_json
		${ITK_LIBRARIES}
		postgres
		postgreslib
		curlpp
)

install(TARGETS test
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
