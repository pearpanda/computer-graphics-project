#include <rg/util/layouts.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace rg::util {

LayoutElement elementf(unsigned int count) {
    return LayoutElement(ElementType::FLOAT, count, false);
}
LayoutElement elementui(unsigned int count) {
    return LayoutElement(ElementType::UNSIGNED_INT, count, false);
}

template <>
LayoutElement element<float>() {
    return elementf(1);
}

template <>
LayoutElement element<unsigned>() {
    return elementui(1);
}

template <>
LayoutElement element<glm::vec2>() {
    return elementf(2);
}

template <>
LayoutElement element<glm::vec3>() {
    return elementf(3);
}

template <>
LayoutElement element<glm::vec4>() {
    return elementf(4);
}

template <>
VertexLayout layout<float>() {
    return {element<float>()};
}

template <>
VertexLayout layout<unsigned>() {
    return {element<unsigned>()};
}

template <>
VertexLayout layout<glm::vec2>() {
    return {element<glm::vec2>()};
}

template <>
VertexLayout layout<glm::vec3>() {
    return {element<glm::vec3>()};
}

template <>
VertexLayout layout<glm::vec4>() {
    return {element<glm::vec4>()};
}

} // namespace rg::util
