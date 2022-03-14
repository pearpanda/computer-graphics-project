add_subdirectory(app)
add_subdirectory(rg)

target_include_directories(${EXECUTABLE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
