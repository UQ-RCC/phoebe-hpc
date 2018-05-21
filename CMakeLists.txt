cmake_minimum_required (VERSION 3.8)

project ("phoebe-hpc")

# setup external targets
include("ImportTargets.cmake")
#debug_targets()
create_boost_target()
create_prostgres_target()

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
message(STATUS "nlohmann_json_FOUND : ${nlohmann_json_FOUND}")
message(STATUS "nlohmann_json_DIR : ${nlohmann_json_DIR}")
print_interface_properties(nlohmann_json)

# Include sub-projects.
add_subdirectory("util")
add_subdirectory("test")
add_subdirectory("sb_loader")