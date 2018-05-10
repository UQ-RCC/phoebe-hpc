# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("phoebe-hpc")

# Include sub-projects.
add_subdirectory ("phoebe-hpc")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY CACHE_VARIABLES)
foreach(dir ${dirs})
   message(STATUS "property '${dir} : ${${dir}}'")
endforeach()

message(STATUS "CMAKE_BINARY_DIR : ${CMAKE_BINARY_DIR}")
