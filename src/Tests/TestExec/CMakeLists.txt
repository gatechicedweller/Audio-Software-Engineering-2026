set(APP TestExec)

set(VibratoLIB Vibrato)
set(AudioLIB AudioFileIO)
set(SndLIB sndlib)

##############################################################
## TestExec

# main test runner
set(TEST_MAIN
        Test.cpp
)

# student test files
file(GLOB TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/src/Tests/Tests/*.cpp
)

# include directories
add_inc_directories()

# build executable
add_executable(${APP}
        ${TEST_MAIN}
        ${TEST_SOURCES}
)

# link libraries
target_link_libraries(${APP}
        GTest::gtest
        GTest::gtest_main
        ${VibratoLIB}
        ${AudioLIB}
        ${SndLIB}
        ${OS_LIBS}
)

# register with CTest
include(GoogleTest)
gtest_discover_tests(TestExec)
