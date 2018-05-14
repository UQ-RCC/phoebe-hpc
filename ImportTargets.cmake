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
		message(STATUS ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
		add_library(boost_${lib} STATIC IMPORTED)
		set_property(TARGET boost_${lib} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
		set_property(TARGET boost_${lib} PROPERTY IMPORTED_LOCATION ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
	endforeach(lib)
	
endfunction()

function(create_fmt_target)
	
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
