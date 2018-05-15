add_executable (test test.cpp test.h)
target_link_libraries(test PRIVATE boost util)

install(TARGETS test
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)