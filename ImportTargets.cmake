# create custom boost target--because FindBoost is such a pain

function(create_boost_target)

	set(BOOST_OPTIONS thread system date_time regex chrono)
	set(BOOST_LIB_PREFIX "libboost_")

	if(WIN32)	
		set(BOOST_ROOT "D:/boost_1_66_0")
		set(BOOST_LIB_SUFFIX "-vc141-mt-x64-1_66.lib")
	else()
		set(BOOST_ROOT $ENV{BOOST_ROOT})
		# Local fl122 sufix set(BOOST_LIB_SUFFIX "-gcc71-mt-x64-1_66.a")
		set(BOOST_LIB_SUFFIX ".a")
	endif()

	find_package(Boost)
	add_library(boost STATIC IMPORTED)
	set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${boost_INCLUDE_DIR})

	foreach(lib ${BOOST_OPTIONS})
		message(STATUS ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
		set_property(TARGET boost PROPERTY IMPORTED_LOCATION ${Boost_LIBRARY_DIRS}/${BOOST_LIB_PREFIX}${lib}${BOOST_LIB_SUFFIX})
	endforeach(lib)
	
endfunction()

function(print_cache)
	get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY CACHE_VARIABLES)
	foreach(dir ${dirs})
		message(STATUS "property '${dir} : ${${dir}}'")
	endforeach()	
endfunction()
