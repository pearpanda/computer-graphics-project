add_subdirectory(objects)

target_sources(${EXECUTABLE} PRIVATE
        cleanup.hpp
        constants.hpp
        init.hpp
        loop.hpp
        state.hpp)
