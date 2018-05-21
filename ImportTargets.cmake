# create custom boost target--because FindBoost is such a pain

function(create_boost_target)

	set(BOOST_OPTIONS thread system date_time regex chrono program_options filesystem)	
	set(BOOST_LIB_PREFIX "libboost_")

	if(WIN32)	
		set(BOOST_ROOT "D:/boost_1_66_0")
		set(BOOST_LIB_SUFFIX "-vc141-mt-x64-1_66.lib")
	else()
		set(BOOST_ROOT $ENV{BOOST_ROOT})		
		set(BOOST_LIB_SUFFIX ".a")
	endif()

	find_package(Boost)
	message(STATUS "boost include dir : ${Boost_INCLUDE_DIRS}")

	add_library(boost INTERFACE IMPORTED)
	set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})

	foreach(lib ${BOOST_OPTIONS})
		#message(STATUS ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
		add_library(boost_${lib} STATIC IMPORTED)
		set_property(TARGET boost_${lib} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
		set_property(TARGET boost_${lib} PROPERTY IMPORTED_LOCATION ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
	endforeach(lib)
	
endfunction()

function(create_prostgres_target)

	find_package(PostgreSQL MODULE REQUIRED)
	message(STATUS "PostgreSQL_INCLUDE_DIRS : ${PostgreSQL_INCLUDE_DIRS}")
	message(STATUS "PostgreSQL_LIBRARY_DIRS : ${PostgreSQL_LIBRARY_DIRS}")
	message(STATUS "PostgreSQL_LIBRARIES : ${PostgreSQL_LIBRARIES}")

	if(WIN32)
		add_library(postgres SHARED IMPORTED)
		set_property(TARGET postgres PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PostgreSQL_INCLUDE_DIRS})
		set_property(TARGET postgres PROPERTY IMPORTED_LOCATION ${PostgreSQL_LIBRARY_DIRS}/libpq.dll)
		set_property(TARGET postgres PROPERTY IMPORTED_IMPLIB ${PostgreSQL_LIBRARY_DIRS}/libpq.lib)
	else()
		add_library(postgres STATIC IMPORTED)
		set_property(TARGET postgres PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PostgreSQL_INCLUDE_DIRS})
		set_property(TARGET postgres PROPERTY IMPORTED_LOCATION ${PostgreSQL_LIBRARY_DIRS}/libpq.a)
	endif()
	
endfunction()

function(create_itk_target)
	find_package(ITK REQUIRED)
	message(STATUS "ITK_DIR : ${ITK_DIR}")
	message(STATUS "ITK_USE_FILE : ${ITK_USE_FILE}")
endfunction()

function(print_cache)
	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY CACHE_VARIABLES)
	foreach(dir ${dirs})
		message(STATUS "property '${dir} : ${${dir}}'")
	endforeach()	
endfunction()

# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

function(print_properties)
    message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_target_properties tgt)
    if(NOT TARGET ${tgt})
      message("There is no target named '${tgt}'")
      return()
    endif()

    foreach (prop ${CMAKE_PROPERTY_LIST})
        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
    # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
    if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
        continue()
    endif()
        # message ("Checking ${prop}")
        get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
        if (propval)
            get_target_property(propval ${tgt} ${prop})
            message ("${tgt} ${prop} = ${propval}")
        endif()
    endforeach(prop)
endfunction(print_target_properties)

function(print_interface_properties tgt)
    if(NOT TARGET ${tgt})
      message("There is no target named '${tgt}'")
      return()
    endif()

    foreach (prop ${CMAKE_PROPERTY_LIST})
        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
    # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i    
	if(NOT (prop MATCHES "^INTERFACE_" 
	        OR prop MATCHES "^COMPATIBLE_INTERFACE_"
			OR prop MATCHES "^MAP_IMPORTED_CONFIG_"
			OR prop STREQUAL "NAME"
			OR prop STREQUAL "IMPORTED"
			OR prop STREQUAL "EXPORT_NAME")
		)
        continue()
    endif()
        # message ("Checking ${prop}")
        get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
        if (propval)
            get_target_property(propval ${tgt} ${prop})
            message ("${tgt} ${prop} = ${propval}")
        endif()
    endforeach(prop)
endfunction(print_interface_properties)

macro(debug_targets)
	set(CMAKE_DEBUG_TARGET_PROPERTIES
		INCLUDE_DIRECTORIES
		COMPILE_DEFINITIONS  
		PUBLIC_HEADER  
	)
endmacro()
