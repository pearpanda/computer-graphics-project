cmake_minimum_required(VERSION 3.17)
project(stb_image
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(HEADERS
        include/stb/stb_image.h)

add_library(stb_image INTERFACE)
target_include_directories(stb_image
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
