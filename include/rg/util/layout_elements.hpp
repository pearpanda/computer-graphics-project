#ifndef RG_UTIL_LAYOUT_ELEMENTS_HPP
#define RG_UTIL_LAYOUT_ELEMENTS_HPP

#include <rg/renderer/buffer/VertexLayout.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace rg::util {

LayoutElement layoutf(unsigned int count);

template <class T>
LayoutElement element() = delete;

template <>
LayoutElement element<float>();
template <>
LayoutElement element<glm::vec2>();
template <>
LayoutElement element<glm::vec3>();
template <>
LayoutElement element<glm::vec4>();

} // namespace rg::util

#endif // RG_UTIL_LAYOUT_ELEMENTS_HPP
