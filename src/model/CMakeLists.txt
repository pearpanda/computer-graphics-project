set(SOURCE_DIR_MODEL ${CMAKE_CURRENT_SOURCE_DIR})
set(HEADER_DIR_MODEL ${HEADER_DIR}/model)

set(SOURCES
        ${SOURCES}
        ${SOURCE_DIR_MODEL}/Ball.cpp
        ${SOURCE_DIR_MODEL}/Court.cpp
        PARENT_SCOPE)

set(HEADERS
        ${HEADERS}
        ${HEADER_DIR_MODEL}/Ball.hpp
        ${HEADER_DIR_MODEL}/Court.hpp
        PARENT_SCOPE)
