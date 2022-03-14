add_subdirectory(buffer)
add_subdirectory(camera)
add_subdirectory(model)
add_subdirectory(shader)

target_sources(${EXECUTABLE} PRIVATE render.cpp)
