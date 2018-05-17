find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable (test src/test.cpp src/test.h)
target_link_libraries(test
	PRIVATE
		boost
		util
		fmt::fmt
		nlohmann_json
		${ITK_LIBRARIES}
		postgres
)

install(TARGETS test
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)