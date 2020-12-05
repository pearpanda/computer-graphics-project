#include <rg/renderer/buffer/VertexLayout.hpp>

#include <glad/glad.h>

#include <numeric>
#include <stdexcept>

namespace rg {

std::size_t LayoutElement::size() const {
    return count * util::size(type);
}

void VertexLayout::push(LayoutElement element) {
    elements_.push_back(element);
    stride_ += element.size();
}

std::vector<unsigned int> VertexLayout::offsets() const {
    if (elements_.empty())
        return {};

    unsigned int n = elements_.size();
    std::vector<unsigned int> offsets(n, 0);
    for (int i = 1; i < n; ++i)
        offsets[i] = offsets[i - 1] + elements_[i - 1].size();

    return offsets;
}

unsigned int VertexLayout::stride() const {
    return stride_;
}

namespace util {

LayoutElement floatVector(unsigned int count) {
    return LayoutElement(ElementType::FLOAT, count, false);
}

LayoutElement uintVector(unsigned int count) {
    return LayoutElement(ElementType::UNSIGNED_INT, count, false);
}

std::size_t size(const ElementType& type) {
    switch (type) {
        case ElementType::FLOAT:
            return sizeof(float);
        case ElementType::UNSIGNED_INT:
            return sizeof(unsigned int);
    }
    throw std::runtime_error{"Element is not FLOAT or UNSIGNED_INT"};
}

unsigned int intValue(const ElementType& type) {
    switch (type) {
        case ElementType::FLOAT:
            return GL_FLOAT;
        case ElementType::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
    }
}

} // namespace util

LayoutElement::LayoutElement(ElementType type, unsigned int count,
                             bool normalized)
        : type{type}, count{count}, normalized{normalized} {
}

VertexLayout::VertexLayout() : elements_{}, stride_{0} {
}

VertexLayout::VertexLayout(std::vector<LayoutElement> elements)
        : elements_{std::move(elements)} {
    stride_ = std::accumulate(
            elements.begin(), elements.end(), 0U,
            [](unsigned int accumulator, const LayoutElement& element) {
                return accumulator + element.size();
            });
}

VertexLayout::VertexLayout(std::initializer_list<LayoutElement> elements)
        : elements_{elements} {
    stride_ = std::accumulate(
            elements.begin(), elements.end(), 0U,
            [](unsigned int accumulator, const LayoutElement& element) {
                return accumulator + element.size();
            });
}

const std::vector<LayoutElement>& VertexLayout::get_elements() const {
    return elements_;
}

} // namespace rg
