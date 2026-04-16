set(CurrLib Vibrato)

# source files
file(GLOB ${CurrLib}_SOURCES
        RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib}
        *.cpp)

# headers (for IDE visibility)
file(GLOB ${CurrLib}_HEADERS
        RELATIVE ${CMAKE_SOURCE_DIR}/src/${CurrLib}
        *.h ../inc/*.h)

# add include directories (legacy helper)
add_inc_directories()

# build library
add_library(${CurrLib} STATIC
        ${${CurrLib}_SOURCES}
        ${${CurrLib}_HEADERS}
)

# export headers for other targets
target_include_directories(${CurrLib}
        PUBLIC ${CMAKE_SOURCE_DIR}/src/inc
)

# output properties
set_target_properties(${CurrLib} PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        OUTPUT_NAME "${CurrLib}"
        PREFIX "lib"
)
