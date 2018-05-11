# CMakeList.txt : CMake project for phoebe-hpc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (phoebehpc "phoebe-hpc.cpp" "phoebe-hpc.h")
target_link_libraries(phoebehpc PRIVATE boost)

# TODO: Add tests and install targets if needed.
