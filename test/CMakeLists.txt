add_executable (test test.cpp test.h)
target_link_libraries(test PRIVATE boost util)