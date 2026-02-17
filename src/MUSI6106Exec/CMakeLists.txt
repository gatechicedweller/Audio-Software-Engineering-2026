set(APP MUSI6106Exec)

set(AudioLIB AudioFileIO)

## get all source files
file(GLOB ${APP}_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.c *.cpp)
file(GLOB ${APP}_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/src/${APP} *.h ../inc/*.h)

## add include directories
add_inc_directories()

## create the target
add_executable(${APP} ${${APP}_SOURCES} ${${APP}_HEADERS})

# modern target-specific includes
target_include_directories(${APP} PRIVATE
        ${PROJECT_INCLUDES}
)

# link the libraries
target_link_libraries(${APP} PRIVATE ${AudioLIB} ${OS_LIBS})
