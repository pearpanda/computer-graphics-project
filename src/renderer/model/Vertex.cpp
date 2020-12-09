#include <rg/renderer/model/Vertex.hpp>

#include <rg/util/layouts.hpp>

namespace rg {

Vertex::Vertex() : position{0.0f}, normal{0.0f}, texture_coordinates{0.0f} {
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal,
               glm::vec2 texture_coordinates)
        : position{position}, normal{normal}, texture_coordinates{
                                                      texture_coordinates} {
}

} // namespace rg

namespace rg::util {

template <>
VertexLayout layout<Vertex>() {
    return {element<glm::vec3>(), element<glm::vec3>(), element<glm::vec2>()};
}

} // namespace rg::util
