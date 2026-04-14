set(CurrLib Fft)

# Get wrapper source files (only from src/Fft)
file(GLOB ${CurrLib}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.cpp)
file(GLOB ${CurrLib}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.h ../inc/*.h)

# Add 3rd party rvfft library
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/Fft "${CMAKE_CURRENT_BINARY_DIR}/rvfft")

## add include directories
add_inc_directories()

# Build a static library
add_library(${CurrLib} STATIC ${${CurrLib}_SOURCES} ${${CurrLib}_HEADERS})

# Set target properties
set_target_properties(${CurrLib} PROPERTIES
		OUTPUT_NAME "${CurrLib}"
		PREFIX "lib"
		CLEAN_DIRECT_OUTPUT 1
)

# Modern target-specific includes
target_include_directories(${CurrLib} PUBLIC
		${CMAKE_SOURCE_DIR}/3rdparty/Fft
		${PROJECT_INCLUDES}
)

# Link the rvfft library
target_link_libraries(${CurrLib} PUBLIC rvfft)

if(WITH_CPPCHECKTARGET)
	# cppcheck target in the IDE
	file(GLOB to_remove RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib} *.txt)
	list(REMOVE_ITEM ${CurrLib}_SOURCES ${to_remove})
	include(GenerateCppcheck)
	GENERATE_CPPCHECK(SOURCES ${${CurrLib}_SOURCES}
			TARGET_NAME "CppCheck_${CurrLib}"
			INCLUDES ${PROJECT_INCLUDES})
endif()
