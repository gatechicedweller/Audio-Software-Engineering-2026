# rvfft library
add_library(rvfft STATIC
        rvfft.cpp
        rvfft.h
)

# Make header available to other targets
target_include_directories(rvfft PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output name
set_target_properties(rvfft PROPERTIES
        OUTPUT_NAME "rvfft"
        PREFIX "lib"
)
