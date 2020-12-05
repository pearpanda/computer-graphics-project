set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include/rg)

set(SOURCES
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/model/Ball.cpp
        ${SOURCE_DIR}/model/Court.cpp
        ${SOURCE_DIR}/renderer/buffer/IndexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexLayout.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexArray.cpp
        ${SOURCE_DIR}/util/layout_elements.cpp)
set(HEADERS
        ${HEADER_DIR}/model/Ball.hpp
        ${HEADER_DIR}/model/Court.hpp
        ${HEADER_DIR}/renderer/buffer/IndexBuffer.hpp
        ${HEADER_DIR}/renderer/buffer/VertexBuffer.hpp
        ${HEADER_DIR}/renderer/buffer/VertexLayout.hpp
        ${HEADER_DIR}/renderer/buffer/VertexArray.hpp
        ${HEADER_DIR}/util/layout_elements.hpp)
set(FILES ${SOURCES} ${HEADERS})

set(EXECUTABLE rg)

# GLFW
find_package(glfw3 REQUIRED)
# GLM
find_package(glm REQUIRED)
# ASSIMP
set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)
find_package(ASSIMP REQUIRED)

# Group the dependencies
set(LIBRARIES
        glfw
        glad
        glm::glm
        assimp
        stb_image)

add_executable(${EXECUTABLE}
        ${FILES})

target_include_directories(${EXECUTABLE}
        PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${EXECUTABLE}
        PRIVATE ${LIBRARIES})
target_compile_definitions(${EXECUTABLE}
        PRIVATE GLFW_INCLUDE_NONE)
