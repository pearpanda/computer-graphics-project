cmake_minimum_required(VERSION 3.17)
project(glad
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/glad.c)
set(HEADERS
        include/glad/glad.h
        include/KHR/khrplatform.h)
set(FILES ${SOURCES} ${HEADERS})

set(LIBRARIES
        dl)

add_library(glad STATIC ${FILES})

target_include_directories(glad PUBLIC include)
target_link_libraries(glad PRIVATE ${LIBRARIES})
