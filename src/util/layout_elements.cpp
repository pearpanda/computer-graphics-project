#include <rg/util/layout_elements.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace rg::util {

LayoutElement layoutf(unsigned int count) {
    return LayoutElement(ElementType::FLOAT, count, false);
}

template <>
LayoutElement element<float>() {
    return layoutf(1);
}

template <>
LayoutElement element<glm::vec2>() {
    return layoutf(2);
}

template <>
LayoutElement element<glm::vec3>() {
    return layoutf(3);
}

template <>
LayoutElement element<glm::vec4>() {
    return layoutf(4);
}

} // namespace util
