cmake_minimum_required(VERSION 3.17)
project(rg LANGUAGES CXX)
set(EXECUTABLE rg)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(RESOURCE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/res)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(spdlog REQUIRED)

# Group the dependencies
set(LIBRARIES
        glfw
        glad
        glm::glm
        assimp
        stb_image
        spdlog::spdlog)

add_executable(${EXECUTABLE})
target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARIES})
target_compile_definitions(${EXECUTABLE}  PRIVATE
        GLFW_INCLUDE_NONE
        RESOURCE_DIRECTORY="${PROJECT_BINARY_DIR}/res")

add_subdirectory(external)
add_subdirectory(include)
add_subdirectory(src)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/res
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/res)
