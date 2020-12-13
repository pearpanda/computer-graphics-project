#ifndef RG_RENDERER_MODEL_VERTEX_HPP
#define RG_RENDERER_MODEL_VERTEX_HPP

#include <rg/renderer/buffer/VertexLayout.hpp>
#include <rg/util/layouts.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace rg {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;

    Vertex();
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates);
};

} // namespace rg

namespace rg::util {

template <>
VertexLayout layout<Vertex>();

} // namespace rg::util

#endif // RG_RENDERER_MODEL_VERTEX_HPP
