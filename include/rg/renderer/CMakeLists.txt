add_subdirectory(buffer)
add_subdirectory(camera)
add_subdirectory(light)
add_subdirectory(model)
add_subdirectory(shader)

target_sources(${EXECUTABLE} PRIVATE
        render.hpp)
