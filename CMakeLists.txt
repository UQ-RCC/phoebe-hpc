cmake_minimum_required (VERSION 3.8)

project ("phoebe-hpc")

# setup external targets
include("ImportTargets.cmake")
#debug_targets()
create_boost_target()
create_prostgres_target()
create_curl_target()

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory("util")
add_subdirectory("database_lib")
add_subdirectory("sb_loader")
add_subdirectory("test")
