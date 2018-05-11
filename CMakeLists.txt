# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("phoebe-hpc")

set(CMAKE_DEBUG_TARGET_PROPERTIES
  INCLUDE_DIRECTORIES
  COMPILE_DEFINITIONS  
  PUBLIC_HEADER  
)

include("ImportTargets.cmake")
create_boost_target()

# Include sub-projects.
add_subdirectory ("phoebe-hpc")

#print_cache()

