# CMakeList.txt : CMake project for phoebe-hpc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (test "test.cpp" "test.h")
target_link_libraries(test PRIVATE boost)
