#ifndef RG_UTIL_LAYOUT_ELEMENTS_HPP
#define RG_UTIL_LAYOUT_ELEMENTS_HPP

#include <rg/renderer/buffer/VertexLayout.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace rg::util {

template <class T>
VertexLayout layout() = delete;

LayoutElement elementf(unsigned int count);
LayoutElement elementui(unsigned int count);

template <class T>
LayoutElement element() = delete;

template <>
LayoutElement element<float>();
template <>
LayoutElement element<unsigned>();
template <>
LayoutElement element<glm::vec2>();
template <>
LayoutElement element<glm::vec3>();
template <>
LayoutElement element<glm::vec4>();

template <>
VertexLayout layout<float>();
template <>
VertexLayout layout<glm::vec2>();
template <>
VertexLayout layout<glm::vec3>();
template <>
VertexLayout layout<glm::vec4>();

} // namespace rg::util

#endif // RG_UTIL_LAYOUT_ELEMENTS_HPP
