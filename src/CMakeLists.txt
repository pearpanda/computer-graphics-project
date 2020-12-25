set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/res)

set(SOURCES
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/renderer/buffer/IndexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexBuffer.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexLayout.cpp
        ${SOURCE_DIR}/renderer/buffer/VertexArray.cpp
        ${SOURCE_DIR}/renderer/buffer/FrameBuffer.cpp
        ${SOURCE_DIR}/renderer/camera/Surface.cpp
        ${SOURCE_DIR}/util/layouts.cpp
        ${SOURCE_DIR}/renderer/shader/Shader.cpp
        ${SOURCE_DIR}/renderer/shader/Shader_set.cpp
        ${SOURCE_DIR}/renderer/model/Vertex.cpp
        ${SOURCE_DIR}/renderer/model/Texture.cpp
        ${SOURCE_DIR}/renderer/model/Mesh.cpp
        ${SOURCE_DIR}/renderer/model/Model.cpp
        ${SOURCE_DIR}/renderer/model/Transform.cpp
        ${SOURCE_DIR}/renderer/camera/View.cpp
        ${SOURCE_DIR}/util/common_meshes.cpp
        ${SOURCE_DIR}/renderer/model/Skybox.cpp
        ${SOURCE_DIR}/renderer/model/Cubemap.cpp
        ${SOURCE_DIR}/renderer/render.cpp
        ${SOURCE_DIR}/app/objects/Camera.cpp
        ${SOURCE_DIR}/app/objects/Lamp.cpp
        ${SOURCE_DIR}/app/constants.cpp
        ${SOURCE_DIR}/app/state.cpp
        ${SOURCE_DIR}/app/init.cpp
        ${SOURCE_DIR}/app/graphics.cpp
        ${SOURCE_DIR}/app/callbacks.cpp
        ${SOURCE_DIR}/app/scene.cpp
        ${SOURCE_DIR}/app/loop.cpp
        ${SOURCE_DIR}/app/cleanup.cpp)
set(HEADERS
        ${HEADER_DIR}/rg/renderer/buffer/IndexBuffer.hpp
        ${HEADER_DIR}/rg/renderer/buffer/VertexBuffer.hpp
        ${HEADER_DIR}/rg/renderer/buffer/VertexLayout.hpp
        ${HEADER_DIR}/rg/renderer/buffer/VertexArray.hpp
        ${HEADER_DIR}/rg/renderer/buffer/FrameBuffer.hpp
        ${HEADER_DIR}/rg/renderer/camera/Surface.hpp
        ${HEADER_DIR}/rg/util/layouts.hpp
        ${HEADER_DIR}/rg/renderer/shader/Shader.hpp
        ${HEADER_DIR}/rg/renderer/model/Vertex.hpp
        ${HEADER_DIR}/rg/renderer/model/Texture.hpp
        ${HEADER_DIR}/rg/renderer/model/Mesh.hpp
        ${HEADER_DIR}/rg/renderer/model/Model.hpp
        ${HEADER_DIR}/rg/renderer/model/Transform.hpp
        ${HEADER_DIR}/rg/renderer/camera/View.hpp
        ${HEADER_DIR}/rg/util/common_meshes.hpp
        ${HEADER_DIR}/rg/renderer/model/Skybox.hpp
        ${HEADER_DIR}/rg/renderer/model/Cubemap.hpp
        ${HEADER_DIR}/rg/renderer/light/lights.hpp
        ${HEADER_DIR}/rg/renderer/render.hpp
        ${HEADER_DIR}/app/objects/Camera.hpp
        ${HEADER_DIR}/app/objects/Ball.hpp
        ${HEADER_DIR}/app/objects/Lamp.hpp
        ${HEADER_DIR}/app/objects/Floor.hpp
        ${HEADER_DIR}/app/constants.hpp
        ${HEADER_DIR}/app/state.hpp
        ${HEADER_DIR}/app/init.hpp
        ${HEADER_DIR}/app/loop.hpp
        ${HEADER_DIR}/app/cleanup.hpp)
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
