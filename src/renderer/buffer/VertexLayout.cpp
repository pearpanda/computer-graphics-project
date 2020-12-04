#include <rg/renderer/buffer/VertexLayout.hpp>

#include <glad/glad.h>

#include <numeric>
#include <stdexcept>

namespace rg {

std::size_t LayoutElement::size() const {
    return count * util::size(type);
}

void VertexLayout::push(LayoutElement element) {
    elements.push_back(element);
    currentStride += element.size();
}

std::vector<unsigned int> VertexLayout::offsets() const {
    if (elements.empty())
        return {};

    unsigned int n = elements.size();
    std::vector<unsigned int> offsets(n, 0);
    for (int i = 1; i < n; ++i)
        offsets[i] = offsets[i - 1] + elements[i - 1].size();

    return offsets;
}

unsigned int VertexLayout::stride() const {
    return currentStride;
}

namespace util {

LayoutElement floatVector(unsigned int count) {
    return LayoutElement(ElementType::FLOAT, count, false);
}

LayoutElement uintVector(unsigned int count) {
    return LayoutElement(ElementType::UNSIGNED_INT, count, false);
}

constexpr std::size_t size(const ElementType& type) {
    switch (type) {
        case ElementType::FLOAT:
            return sizeof(float);
        case ElementType::UNSIGNED_INT:
            return sizeof(unsigned int);
    }
    throw std::runtime_error{"Element is not FLOAT or UNSIGNED_INT"};
}

} // namespace util

LayoutElement::LayoutElement(ElementType type, unsigned int count,
                             bool normalized)
        : type{type}, count{count}, normalized{normalized} {
}

VertexLayout::VertexLayout() : elements{}, currentStride{0} {
}

VertexLayout::VertexLayout(std::vector<LayoutElement> elements)
        : elements{std::move(elements)} {
    currentStride = std::accumulate(
            elements.begin(), elements.end(), 0U,
            [](unsigned int accumulator, const LayoutElement& element) {
                return accumulator + element.size();
            });
}

VertexLayout::VertexLayout(std::initializer_list<LayoutElement> elements)
        : elements{elements} {
    currentStride = std::accumulate(
            elements.begin(), elements.end(), 0U,
            [](unsigned int accumulator, const LayoutElement& element) {
                return accumulator + element.size();
            });
}

const std::vector<LayoutElement>& VertexLayout::get_elements() const {
    return elements;
}

} // namespace rg
