cmake_minimum_required (VERSION 3.8)

project ("phoebe-hpc")

# setup external targets
include("ImportTargets.cmake")
create_boost_target()
find_package(fmt CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory("util")
add_subdirectory("test")
add_subdirectory("sb_loader")