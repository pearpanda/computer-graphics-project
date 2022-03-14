add_subdirectory(objects)

target_sources(${EXECUTABLE} PRIVATE
        callbacks.cpp
        cleanup.cpp
        constants.cpp
        graphics.cpp
        init.cpp
        loop.cpp
        scene.cpp
        state.cpp)
