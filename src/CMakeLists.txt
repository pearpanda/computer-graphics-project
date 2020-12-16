set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include/rg)
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/res)

set(SOURCES
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/model/Ball.cpp
        ${SOURCE_DIR}/model/Court.cpp
        ${SOURCE_DIR}/renderer/buffer/IndexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexLayout.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexArray.cpp
        ${SOURCE_DIR}/util/layouts.cpp
        ${SOURCE_DIR}/util/read_file.cpp
        ${SOURCE_DIR}/renderer/shader/Shader.cpp
        ${SOURCE_DIR}/renderer/shader/Shader_set.cpp
        ${SOURCE_DIR}/renderer/model/Vertex.cpp
        ${SOURCE_DIR}/renderer/model/Texture.cpp
        ${SOURCE_DIR}/renderer/model/Mesh.cpp
        ${SOURCE_DIR}/renderer/model/Model.cpp
        ${SOURCE_DIR}/renderer/camera/View.cpp
        ${SOURCE_DIR}/renderer/Renderer.cpp
        ${SOURCE_DIR}/renderer/light/Light.cpp
        ${SOURCE_DIR}/renderer/light/SpotLight.cpp
        ${SOURCE_DIR}/renderer/light/DirectionalLight.cpp
        ${SOURCE_DIR}/renderer/light/PointLight.cpp)
set(HEADERS
        ${HEADER_DIR}/model/Ball.hpp
        ${HEADER_DIR}/model/Court.hpp
        ${HEADER_DIR}/renderer/buffer/IndexBuffer.hpp
        ${HEADER_DIR}/renderer/buffer/VertexBuffer.hpp
        ${HEADER_DIR}/renderer/buffer/VertexLayout.hpp
        ${HEADER_DIR}/renderer/buffer/VertexArray.hpp
        ${HEADER_DIR}/util/layouts.hpp
        ${HEADER_DIR}/util/read_file.hpp
        ${HEADER_DIR}/renderer/shader/Shader.hpp
        ${HEADER_DIR}/renderer/model/Vertex.hpp
        ${HEADER_DIR}/renderer/model/Texture.hpp
        ${HEADER_DIR}/renderer/model/Mesh.hpp
        ${HEADER_DIR}/renderer/model/Model.hpp
        ${HEADER_DIR}/renderer/camera/View.hpp
        ${HEADER_DIR}/renderer/Renderer.hpp
        ${HEADER_DIR}/renderer/light/Light.hpp
        ${HEADER_DIR}/renderer/light/DirectionalLight.hpp
        ${HEADER_DIR}/renderer/light/PointLight.hpp
        ${HEADER_DIR}/renderer/light/SpotLight.hpp)
set(FILES ${SOURCES} ${HEADERS})

set(EXECUTABLE rg)

# GLFW
find_package(glfw3 REQUIRED)
# GLM
find_package(glm REQUIRED)
# ASSIMP
set(CMAKE_POLICY_DEFAULT_CMP0012 NEW)
find_package(ASSIMP REQUIRED)
# SPDLOG
find_package(spdlog)

# Group the dependencies
set(LIBRARIES
        glfw
        glad
        glm::glm
        assimp
        stb_image
        spdlog)

add_executable(${EXECUTABLE}
        ${FILES})

target_include_directories(${EXECUTABLE}
        PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${EXECUTABLE}
        PRIVATE ${LIBRARIES})
target_compile_definitions(${EXECUTABLE}
        PRIVATE
        GLFW_INCLUDE_NONE
        RESOURCE_DIRECTORY="${RESOURCE_DIR}")
# TODO: Change ${RESOURCE_DIR} to ${RESOURCE_OUTPUT_DIR}

# Copy resources to build directory
# message(STATUS "RESOURCE LIST: ${RESOURCE_LIST}")
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${RESOURCE_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_DIR}
        ${RESOURCE_OUTPUT_DIRECTORY})
