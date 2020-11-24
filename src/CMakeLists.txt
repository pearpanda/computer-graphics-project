set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES ${SOURCE_DIR}/main.cpp)
set(HEADERS)
set(FILES ${SOURCES} ${HEADERS})

set(EXECUTABLE rg)

# GLFW
find_package(glfw3 REQUIRED)

# Group the dependencies
set(LIBRARIES
        glfw
        glad)

add_executable(${EXECUTABLE}
        ${FILES})

target_link_libraries(${EXECUTABLE}
        PRIVATE ${LIBRARIES})
target_compile_definitions(${EXECUTABLE}
        PRIVATE GLFW_INCLUDE_NONE)
