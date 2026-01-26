set(CurrLib AudioFileIO)

## set version number
set(${CurrLib}_VERSION_MAJOR "0" CACHE INTERNAL "")
set(${CurrLib}_VERSION_MINOR "0" CACHE INTERNAL "")
set(${CurrLib}_VERSION_PATCH "1" CACHE INTERNAL "")

# get wrapper source files
file(GLOB ${CurrLib}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.c *.cpp)
file(GLOB ${CurrLib}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.h ../inc/*.h)

# get library source files
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/sndlib "${CMAKE_CURRENT_BINARY_DIR}/sndlib")

## add include directories
add_inc_directories()

# build a static library
add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS})

# set target properties
set_target_properties(${CurrLib} PROPERTIES
		OUTPUT_NAME "${CurrLib}"
		PREFIX "lib"
		CLEAN_DIRECT_OUTPUT 1
)

# modern target-specific includes
target_include_directories(${CurrLib} PUBLIC
		${CMAKE_SOURCE_DIR}/3rdparty/sndlib
		${PROJECT_INCLUDES}
)

# link libraries
target_link_libraries(${CurrLib} PUBLIC sndlib)

if(WITH_CPPCHECKTARGET)
	# cppcheck target in the IDE
	file(GLOB to_remove RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.txt)
	list(REMOVE_ITEM ${CurrLib}_SOURCES ${to_remove})
	include(GenerateCppcheck)
	GENERATE_CPPCHECK(SOURCES ${${CurrLib}_SOURCES}
			TARGET_NAME "CppCheck_${CurrLib}"
			INCLUDES ${PROJECT_INCLUDES})
endif()